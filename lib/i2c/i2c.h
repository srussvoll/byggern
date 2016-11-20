#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#include "lib/stream/stream.h"
#include "lib/i2c/pins.h"

// Status byte holding flags.
union TWI_statusReg
{
    uint8_t all;
    struct
    {
        uint8_t lastTransOK:1;
        uint8_t unusedBits:7;
    };
};

extern union TWI_statusReg TWI_statusReg;

ISR(TWI_vect);


/**
 * \brief I2C (TWI) for communicating for peripherals
 *
 * This implementation is based on the http://www.atmel.com/Images/Atmel-2564-Using-the-TWI-Module-as-I2C-Master_ApplicationNote_AVR315.pdf device driver example
 * Most of the implementation is ported from ATMEL/AVRs C IAR example code found in the link.
 * However, we had to port it to C++ and the avrgcc compiler.
 * Note that we were not asked to create this driver, so the implementation is due to ATMEL
 * © ATMEL / Microship
 */
class I2C : Stream {

private:

    /**
     * Default constructor. Private due to singleton
     */
    I2C();

    /**
     * The interrupt handler vector. To be run on each DRE interrupt
     */
    friend void TWI_vect();

public:

    /**
     * The size, in bytes, of the message to be sent
     */
    uint8_t message_size;

    /**
     * A status flag for the TWI - from ATMELS implementation
     */
    uint8_t TWI_state;

    /**
     * A temporarily output buffer
     */
    uint8_t I2C_output_buffer[4];

    /**
     * A general status union (of more statuses)
     */
    union TWI_statusReg TWI_statusReg;

    /**
     * A Singleton implementation of this class
     */
    static I2C& GetInstance(){
        static I2C instance;
        return instance;
    }

    /**
     * Call this function to set up the TWI master to its initial standby state.
     * Remember to enable interrupts from the main application after initializing the TWI.
     * @param baudrate The baudrate of the system. Should be set to 0x0C
     */
    void Initialize(uint8_t baudrate);

    /**
     * Call this function to test if the TWI_ISR is busy transmitting.
     */
    bool TransceiverIsBusy();

    /**
     * Call this function to fetch the state information of the previous operation. The function will hold execution (loop)
     * until the TWI_ISR has completed with the previous operation. If there was an error, then the function
     * will return the TWI State code.
     */
    uint8_t GetStateInfo();

    /**
     * Call this function to send a prepared message. The first byte must contain the slave address and the
     * read/write bit. Consecutive bytes contain the data to be sent, or empty locations for data to be read
     * from the slave. Also include how many bytes that should be sent/read including the address byte.
     * The function will hold execution (loop) until the TWI_ISR has completed with the previous operation,
     * @param message The bytes to be sent
     * @param message_size The size of the byte package
     * then initialize the next operation and return.
     */
    void SendData(uint8_t *message, uint8_t message_size);

    /**
     * Call this function to resend the last message. The driver will reuse the data previously put in the transceiver buffers.
     * The function will hold execution (loop) until the TWI_ISR has completed with the previous operation,
     * then initialize the next operation and return.
     */
    void StartTransceiver();

    /**
     * Call this function to read out the requested data from the TWI transceiver buffer. I.e. first call
     * TWI_Start_Transceiver to send a request for data to the slave. Then Run this function to collect the
     * data when they have arrived. Include a pointer to where to place the data and the number of bytes
     * requested (including the address field) in the function call. The function will hold execution (loop)
     * until the TWI_ISR has completed with the previous operation, before reading out the data and returning.
     * If there was an error in the previous transmission the function will return the TWI error code.
     * @param message A pointer to where to place the data
     * @param message_size Number of bytes requested
     * @return The I2C error code
     */
    uint8_t GetDataFromTransceiver(uint8_t *message, uint8_t message_size);

};
