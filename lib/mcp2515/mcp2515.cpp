#include <stdint.h>
#include "mcp2515.h"
#include "mcp2515_regisers.h"
#include "../utilities/printf.h"
#include "../uart/uart.h"
#include <util/delay.h>
void MCP2515_INT(){
    sei();
    MCP2515 &mcp = MCP2515::GetInstance();
    uint8_t interrupt_flags;

    // Get the interrupt flags from the MCP2515
    mcp.ReadFromRegister(MCP_CANINTF, interrupt_flags);

    // If interrupt is transmit
    if(interrupt_flags & (MCP_TX0IF)) {
        // Transmission is complete and TX buffer 1 is now empty
        mcp.clear_to_send = true;

        // Clear the interrupt
        uint8_t bitmask = (MCP_TX0IF);
        mcp.BitModify(MCP_CANINTF, bitmask, 0x00);
    }

    // If interrupt is recieve
    if(interrupt_flags & (MCP_RX0IF)) {
        uint8_t data_to_rec[8];
        CAN_MESSAGE rec_mess = CAN_MESSAGE(0,data_to_rec,0);

        // Read the data from the MCP2515
        mcp.ReadRxFrame(rec_mess);

        // Pass the data one layer up. In this case it is the SOCKET class
        (*mcp.upper_level)(rec_mess);

        // Clear the interrupt in the MCP2515
        uint8_t bitmask = (MCP_RX0IF);
        mcp.BitModify(MCP_CANINTF, bitmask, 0x00);
    }
}

void MCP2515::Initialize(SPI_N::SPI *spi, uint16_t identifier) {
    this->spi_driver = spi;

    // Initialize the interrupt
    sei();
#ifdef __AVR_ATmega162__

    GICR |= (1 << INT0);

    // Falling edge
    MCUCR |= (2<<ISC00);

#elif __AVR_ATmega2560__

    // Falling edge
    EICRB |= (2<<ISC40);

    // Enable the interrupt
    EIMSK |= (1<<INT4);

#endif

    // Assuming the SPI is already initialized
    this->Reset();

    // Set interrupts in the MCP2515
    this->WriteToRegister(MCP_CANINTE, 0b00000101);

    // Set receive filter
    uint8_t upper_id = (uint8_t) (identifier >> 3);
    uint8_t lower_id = (uint8_t) identifier & 0b111;
    this->WriteToRegister(MCP_RXF0SIDL, (lower_id<<5));
    this->WriteToRegister(MCP_RXF0SIDH, upper_id);

    // Set the receive mask. Currently 0x00, aka we accept every message.
    this->WriteToRegister(MCP_RXM0SIDH, 0x00);
    this->WriteToRegister(MCP_RXM0SIDL, (0x00 << 5));
}

void MCP2515::RequestToSend() {
    this->spi_driver->WriteByteAndThrowAwayData(0b10000001, 0);
}

void MCP2515::Reset() {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_RESET, 0);
}

void MCP2515::BitModify(uint8_t register_address, uint8_t mask, uint8_t data) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_BITMOD, 1);
    this->spi_driver->WriteByteAndThrowAwayData(register_address, 1);
    this->spi_driver->WriteByteAndThrowAwayData(mask, 1);
    this->spi_driver->WriteByteAndThrowAwayData(data, 0);
}

void MCP2515::SetLoopback() {
    uint8_t bitmask = (1<<MCP_REQOP1) | (1<<MCP_REQOP0) | (1<<MCP_REQOP2);
    uint8_t data = (1<<MCP_REQOP1); // MCP_REQOP1 = 1, rest = 0
    this->BitModify(MCP_CANCTRL, bitmask, data);
}

void MCP2515::WriteToRegister(uint8_t register_address, uint8_t byte) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_WRITE, 1);
    this->spi_driver->WriteByteAndThrowAwayData(register_address, 1);
    this->spi_driver->WriteByteAndThrowAwayData(byte, 0);
}

void MCP2515::ReadFromRegister(uint8_t register_address, uint8_t &byte) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_READ, 1);
    this->spi_driver->WriteByteAndThrowAwayData(register_address, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(!this->spi_driver->ReadByte(byte));
}

void MCP2515::ReadStatus(uint8_t &byte) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_READ_STATUS, 1);
    this->spi_driver->WriteByteAndThrowAwayData(MCP_DC,1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(!this->spi_driver->ReadByte(byte));
}

void MCP2515::SetNormal() {
    uint8_t bitmask = (1 << MCP_REQOP1) | (1 << MCP_REQOP0) | (1 << MCP_REQOP2);
    uint8_t data = 0x00;
    this->BitModify(MCP_CANCTRL, bitmask, data);
}

void MCP2515::LoadTxFrame(CAN_MESSAGE &message) {
    /*
     * We cannot load another TX buffer if we are not clear to send, since we only use one buffer. We therefor need to
     * wait in order to send a new one.
     */
    while(!this->clear_to_send);
    this->clear_to_send = false;

    // We need the first 11 bits of ID
    uint8_t upper_id = (uint8_t) (message.id >> 3);
    uint8_t lower_id = (uint8_t) message.id & 0b111;

    // Get the two addresses.
    uint8_t buffer_number = 0;
    uint8_t address_sidh = 0x40 | (buffer_number);

    // Set the MCP2515 register pointer to TXB0SIDH
    this->spi_driver->WriteByteAndThrowAwayData(address_sidh, 1);
    this->spi_driver->WriteByteAndThrowAwayData(upper_id, 1);
    this->spi_driver->WriteByteAndThrowAwayData((lower_id<<5), 1);
    this->spi_driver->WriteByteAndThrowAwayData(MCP_DC, 1);
    this->spi_driver->WriteByteAndThrowAwayData(MCP_DC, 1);
    this->spi_driver->WriteByteAndThrowAwayData((message.size & 0x0F), 1);

    //this->spi_driver->WriteByteAndThrowAwayData(address_data, 1);
    for(int i = 0; i < message.size; i++){
        if(i == (message.size - 1)){
            this->spi_driver->WriteByteAndThrowAwayData(message.data[i], 0);
        }else{
            this->spi_driver->WriteByteAndThrowAwayData(message.data[i], 1);
        }

    }
}

void MCP2515::RxStatus(uint8_t &byte) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_RX_STATUS,1);
    this->spi_driver->WriteByteAndThrowAwayData(MCP_DC, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(!this->spi_driver->ReadByte(byte));
}

void MCP2515::ReadRxFrame(CAN_MESSAGE &message) {
    uint8_t buffer_number = 0;
    uint8_t address_sidh = 0b10010000 | (buffer_number << 2);
    uint8_t address_data = 0b10010010 | (buffer_number << 2);
    uint8_t address_dlc = 0b01100101 | (buffer_number << 4);

    // Read length. Not using "clocked read" due to not needing RXB0EID8 and RXB0EID0.
    uint8_t size;
    this->ReadFromRegister(address_dlc, size);
    message.size = (size & 0x0F);

    // Read identifier of the message
    uint8_t upper_id;
    uint8_t lower_id;
    this->spi_driver->WriteByteAndThrowAwayData(address_sidh, 1);
    this->spi_driver->WriteByte(MCP_DC, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);

    while(!this->spi_driver->ReadByte(upper_id));
    while(!this->spi_driver->ReadByte(lower_id));

    message.id = (upper_id << 3) + (lower_id >> 5);

    // Read data. Using "clocked read"
    this->spi_driver->WriteByteAndThrowAwayData(address_data, 1);
    for(int i = 0; i < size; i++) {
        if(i == (size - 1)) {
            // Last byte ready for transmission. Start it with wait = 0
            this->spi_driver->WriteByte(MCP_DC, 0);
        }else {
            // Still more bytes to be loaded in the buffer before transmission. Set wait = 1
            this->spi_driver->WriteByte(MCP_DC, 1);
        }
    }

    // Until all of the data has been received
    while(spi_driver->GetAvailableReadBytes() < size);

    // Read the data from the spi driver and put them into the CAN message
    for(int i = 0; i < size; i++) {
        this->spi_driver->ReadByte(message.data[i]);
    }
}

void MCP2515::SendMessage(CAN_MESSAGE &message) {
// Turn of interrupt during transmission. This is due to using sei() in the interrupt handler
#ifdef __AVR_ATmega162__
    GICR &= ~(1 << INT0);
    this->LoadTxFrame(message);
    this->RequestToSend();
    GICR |= (1 << INT0);
#elif __AVR_ATmega2560__
    EIMSK &= ~(1 << INT4);
    this->LoadTxFrame(message);
    this->RequestToSend();
    EIMSK |= (1<<INT4);
#endif
}
