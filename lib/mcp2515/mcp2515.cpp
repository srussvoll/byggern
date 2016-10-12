#include <stdint.h>
#include "mcp2515.h"
#include "mcp2515_regisers.h"
#include "../utilities/printf.h"
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
    printf("Setting data = %2x", data);
}

void MCP2515::Initialize(SPI_N::SPI *spi) {
    this->spi_driver = spi;

    // Assuming the SPI is already initialized

    this->Reset();

    // MCP2515 is now in config mode. Must manually be set to another mode

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
    this->spi_driver->WriteByte(MCP_DC,0);
    this->spi_driver->WriteByteAndThrowAwayData(MCP_DC, 0);
    this->spi_driver->ReadByte(byte);
}

void MCP2515::SetNormal() {
    uint8_t bitmask = (1<<MCP_REQOP1) | (1<<MCP_REQOP0) | (1<<MCP_REQOP2);
    uint8_t data = 0x00; // All 0
    this->BitModify(MCP_CANCTRL, bitmask, data);
}
