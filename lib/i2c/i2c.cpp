// FIXME: (finish) Remove this.
#if DEBUG
#define __AVR_ATmega2560__
#endif

#ifdef __AVR_ATmega2560__

#include "i2c.h"

I2C::I2C() : Stream(64, 64){
    this->TWI_state = TWI_NO_STATE;
    this->TWI_statusReg = {0};
}

void TWI_vect() {
    static unsigned char TWI_bufPtr;

    I2C& i2c = I2C::GetInstance();

    switch (TWSR)
    {
        case TWI_START:             // START has been transmitted
        case TWI_REP_START:         // Repeated START has been transmitted
            TWI_bufPtr = 0;                                     // Set buffer pointer to the TWI Address location
        case TWI_MTX_ADR_ACK:       // SLA+W has been tramsmitted and ACK received
        case TWI_MTX_DATA_ACK:      // Data byte has been tramsmitted and ACK received
            if (TWI_bufPtr < i2c.message_size)
            {
                TWDR = i2c.I2C_output_buffer[TWI_bufPtr++];
                TWCR = (1<<TWEN)|                                 // TWI Interface enabled
                       (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to send byte
                       (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           //
                       (0<<TWWC);                                 //
            }else                    // Send STOP after last byte
            {
                i2c.TWI_statusReg.lastTransOK = TRUE;                 // Set status bits to completed successfully.
                TWCR = (1<<TWEN)|                                 // TWI Interface enabled
                       (0<<TWIE)|(1<<TWINT)|                      // Disable TWI Interrupt and clear the flag
                       (0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|           // Initiate a STOP condition.
                       (0<<TWWC);                                 //
            }
            break;
        case TWI_MRX_DATA_ACK:      // Data byte has been received and ACK tramsmitted
            i2c.I2C_output_buffer[TWI_bufPtr++] = TWDR;
        case TWI_MRX_ADR_ACK:       // SLA+R has been tramsmitted and ACK received
            if (TWI_bufPtr < (i2c.message_size-1) )                  // Detect the last byte to NACK it.
            {
                TWCR = (1<<TWEN)|                                 // TWI Interface enabled
                       (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to read next byte
                       (1<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send ACK after reception
                       (0<<TWWC);                                 //
            }else                    // Send NACK after next reception
            {
                TWCR = (1<<TWEN)|                                 // TWI Interface enabled
                       (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag to read next byte
                       (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // Send NACK after reception
                       (0<<TWWC);                                 //
            }
            break;
        case TWI_MRX_DATA_NACK:     // Data byte has been received and NACK tramsmitted
            i2c.I2C_output_buffer[TWI_bufPtr] = TWDR;
            i2c.TWI_statusReg.lastTransOK = TRUE;                 // Set status bits to completed successfully.
            TWCR = (1<<TWEN)|                                 // TWI Interface enabled
                   (0<<TWIE)|(1<<TWINT)|                      // Disable TWI Interrupt and clear the flag
                   (0<<TWEA)|(0<<TWSTA)|(1<<TWSTO)|           // Initiate a STOP condition.
                   (0<<TWWC);                                 //
            break;
        case TWI_ARB_LOST:          // Arbitration lost
            TWCR = (1<<TWEN)|                                 // TWI Interface enabled
                   (1<<TWIE)|(1<<TWINT)|                      // Enable TWI Interupt and clear the flag
                   (0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|           // Initiate a (RE)START condition.
                   (0<<TWWC);                                 //
            break;
        case TWI_MTX_ADR_NACK:      // SLA+W has been tramsmitted and NACK received
        case TWI_MRX_ADR_NACK:      // SLA+R has been tramsmitted and NACK received
        case TWI_MTX_DATA_NACK:     // Data byte has been tramsmitted and NACK received
//    case TWI_NO_STATE              // No relevant state information available; TWINT = �0�
        case TWI_BUS_ERROR:         // Bus error due to an illegal START or STOP condition
        default:
        i2c.TWI_state = TWSR;                                 // Store TWSR and automatically sets clears noErrors bit.
            // Reset TWI Interface
            TWCR = (1<<TWEN)|                                 // Enable TWI-interface and release TWI pins
                   (0<<TWIE)|(0<<TWINT)|                      // Disable Interupt
                   (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests
                   (0<<TWWC);                                 //
    }
}

void I2C::Initialize(uint8_t baudrate) {
    // Set bit rate register (Baudrate). Defined in header file.
    TWBR = baudrate;

    // Default content = SDA released.
    TWDR = 0xFF;

    // Enable TWI-interface and release TWI pins. Disable Interupt. No Signal requests.
    TWCR = (1<<TWEN)|(0<<TWIE)|(0<<TWINT)|(0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
}

bool I2C::TransceiverIsBusy() {
    // IF TWI Interrupt is enabled then the Transceiver is busy
    return ( TWCR & (1<<TWIE) );
}

uint8_t I2C::GetStateInfo() {
    // Wait until TWI has completed the transmission.
    while (TransceiverIsBusy());

    // Return error state.
    return ( TWI_state );
}

void I2C::SendData(uint8_t *message, uint8_t message_size) {

    uint8_t temp;

    // Wait until TWI is ready for next transmission.
    while ( TransceiverIsBusy() );

    // Number of data to transmit.
    this->message_size = message_size;

    // Store slave address with R/W setting.
    this->I2C_output_buffer[0]  = message[0];

    if (!( message[0] & (TRUE<<TWI_READ_BIT) ))       // If it is a write operation, then also copy data.
    {
        for ( temp = 1; temp < message_size; temp++ )
            this->I2C_output_buffer[ temp ] = message[ temp ];
    }
    TWI_statusReg.all = 0;
    TWI_state         = TWI_NO_STATE ;

    // TWI Interface enabled. Enable TWI Interupt and clear the flag. Initiate a START condition.
    TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC);
}

void I2C::StartTransceiver() {

    // Wait until TWI is ready for next transmission.
    while (TransceiverIsBusy());

    this->TWI_statusReg.all = 0;

    this->TWI_state         = TWI_NO_STATE ;

    // TWI Interface enabled. Enable TWI Interupt and clear the flag. Initiate a START condition.
    TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(0<<TWEA)|(1<<TWSTA)|(0<<TWSTO)|(0<<TWWC);

}

uint8_t I2C::GetDataFromTransceiver(uint8_t *message, uint8_t message_size) {
    uint8_t i;

    // Wait until TWI is ready for next transmission.
    while (TransceiverIsBusy());

    // Last transmission completed successfully.
    if( this->TWI_statusReg.lastTransOK )
    {
        // Copy data from Transceiver buffer.
        for(i = 0; i < message_size; i++)
        {
            message[i] = this->I2C_output_buffer[i];
        }
    }
    return( this->TWI_statusReg.lastTransOK );
}

#endif