#include <stdint.h>
#include "mcp2515.h"
#include "mcp2515_regisers.h"
#include "../utilities/printf.h"
void MCP2515_INT(){
    MCP2515 &mcp = MCP2515::GetInstance();
    uint8_t read_status;
    mcp.RxStatus(read_status);
    bool read_zero = false;
    bool read_one = false;
    if(read_status & 0b11000000){
        read_zero = true;
        read_one = true;
    }else if(read_status & 0b10000000){
        // RXB1
        read_one = true;

    }else if(read_status & 0b010000000){
        // RXB0
        read_zero = true;
    }

    if(read_zero){
        // Clear the interrupt
        uint8_t bitmask = (1<<MCP_RX0IF);
        mcp.BitModify(MCP_CANINTF, bitmask, 0x00);

    }
    if(read_one){
        // Clear the interrupt
        uint8_t bitmask = (1<<MCP_RX1IF);
        mcp.BitModify(MCP_CANINTF, bitmask, 0x00);
    }

}
void MCP2515::RequestToSend() {
    // TODO: Implement. 0b100000000 is wrong
    this->spi_driver->WriteByteAndThrowAwayData(0b10000000, 0);
}

void MCP2515::Reset() {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_RESET, 0); // 1100 0000
}

void MCP2515::BitModify(uint8_t register_address, uint8_t mask, uint8_t data) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_BITMOD, 1); // Instruction
    this->spi_driver->WriteByteAndThrowAwayData(register_address, 1);
    this->spi_driver->WriteByteAndThrowAwayData(mask, 1);
    this->spi_driver->WriteByteAndThrowAwayData(data, 0);
}

void MCP2515::Initialize(SPI_N::SPI *spi, SPI_N::PIN *interrupt_pin) {
    this->spi_driver = spi;
    this->interrupt_pin = interrupt_pin;
    sei();
    *this->interrupt_pin->ddr &= ~(1<<this->interrupt_pin->pin);
    GICR |= (1 << INT0);
    MCUCR |= (1<<ISC01);
    MCUCR &= ~(1<<ISC00);


    // Assuming the SPI is already initialized

    this->Reset();

    // Set interrupts
    this->WriteToRegister(MCP_CANINTE, 0b00000011);

}

void MCP2515::SetLoopback() {
    uint8_t bitmask = (1<<MCP_REQOP1) | (1<<MCP_REQOP0) | (1<<MCP_REQOP2);
    uint8_t data = (1<<MCP_REQOP1); // MCP_REQOP1 = 1, rest = 0
    this->BitModify(MCP_CANCTRL, bitmask, data);
}

void MCP2515::WriteToRegister(uint8_t register_address, uint8_t byte) {
    // Page 67 of datasheet
    this->spi_driver->WriteByteAndThrowAwayData(MCP_WRITE, 1); // Instruction
    this->spi_driver->WriteByteAndThrowAwayData(register_address, 1);
    this->spi_driver->WriteByteAndThrowAwayData(byte, 0);
}

void MCP2515::ReadFromRegister(uint8_t register_address, uint8_t &byte) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_READ, 1); // Instruction
    this->spi_driver->WriteByteAndThrowAwayData(register_address, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(spi_driver->GetAvailableReadBytes() == 0);
    this->spi_driver->ReadByte(byte);
}

void MCP2515::ReadStatus(uint8_t &byte) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_READ_STATUS, 1);
    this->spi_driver->WriteByteAndThrowAwayData(MCP_DC,1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(spi_driver->GetAvailableReadBytes() == 0);
    this->spi_driver->ReadByte(byte);
}

void MCP2515::SetNormal() {
    uint8_t bitmask = (1<<MCP_REQOP1) | (1<<MCP_REQOP0) | (1<<MCP_REQOP2);
    uint8_t data = 0x00; // All 0
    this->BitModify(MCP_CANCTRL, bitmask, data);
}

void MCP2515::LoadTxFrame(CAN_MESSAGE &message, uint8_t buffer_number) {


    // We need the first 11 bits of ID
    uint8_t upper_id = (uint8_t) (message.id >> 3);
    uint8_t lower_id = (uint8_t) message.id & 0b111;
    // Get the two addresses.
    uint8_t address_sidh = 0x40 | (buffer_number);
    uint8_t address_data = 0x40 | (buffer_number + 1);
    uint8_t address_sidl = ((0b11 + buffer_number) << 4) + 0b10;
    uint8_t address_dlc = ((0b11 + buffer_number) << 4) + 0b101;

    this->spi_driver->WriteByteAndThrowAwayData(address_sidh, 1);
    this->spi_driver->WriteByteAndThrowAwayData(upper_id, 0);
    this->spi_driver->WriteByteAndThrowAwayData(address_data, 1);
    this->spi_driver->WriteByteAndThrowAwayData(message.data, 0);
    this->WriteToRegister(address_dlc, (message.data & 0x0F));
    this->WriteToRegister(address_sidl, (lower_id<<5));
    printf("Done sending\n");
}

void MCP2515::RxStatus(uint8_t &byte) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_RX_STATUS,1);
    this->spi_driver->WriteByteAndThrowAwayData(MCP_DC, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(spi_driver->GetAvailableReadBytes() == 0);
    this->spi_driver->ReadByte(byte);
}

void MCP2515::ReadRxFrame(CAN_MESSAGE &message, uint8_t buffer_number) {
    uint8_t address_sidh = 0b10010000 | (buffer_number << 2);
    uint8_t address_data = 0b10010010 | (buffer_number << 2);
    uint8_t address_sidl = 0b01100010 | (buffer_number << 4);

    this->spi_driver->WriteByteAndThrowAwayData(address_data, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(spi_driver->GetAvailableReadBytes() == 0);
    this->spi_driver->ReadByte(message.data);

    this->spi_driver->WriteByteAndThrowAwayData(address_sidh, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(spi_driver->GetAvailableReadBytes() == 0);
    //TODO: YOU STOPPED HERE WHEN NO MORE COFFEE :(
}
