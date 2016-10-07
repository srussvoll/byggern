#include <stdint.h>
#include "mcp2515.h"
#include "mcp2515_regisers.h"
void MCP2515::RequestToSend() {
    this->spi_driver->WriteByte(0x02, 0);
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

void MCP2515::initialize(SPI_N::SPI *spi, SPI_N::PIN ss_pin) {
    this->ss_pin = ss_pin;
    this->spi_driver = spi;

    // Assuming the SPI is already initialized

    this->Reset();

    // MCP2515 is now in config mode.

    // TODO: Set F_SCK
}

void MCP2515::SetLoopback() {
    // TODO: Implement
}

void MCP2515::WriteToRegister(uint8_t register_address, uint8_t byte) {
    // Page 67 of datasheet
    this->spi_driver->WriteByteAndThrowAwayData(MCP_WRITE, 1); // Instruction
    this->spi_driver->WriteByteAndThrowAwayData(register_address, 1);
    this->spi_driver->WriteByteAndThrowAwayData(byte, 0);
}

uint8_t MCP2515::ReadFromRegister(uint8_t register_address) {
    uint8_t data;
    this->spi_driver->WriteByteAndThrowAwayData(MCP_READ, 1); // Instruction
    this->spi_driver->WriteByteAndThrowAwayData(register_address, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    this->spi_driver->ReadByte(data);
    return data;
}

uint8_t MCP2515::ReadStatus() {
    this->spi_driver->WriteByte(MCP_READ_STATUS, 1);
    uint8_t data;
    this->spi_driver->ReadByte(data);
    return data;
}
