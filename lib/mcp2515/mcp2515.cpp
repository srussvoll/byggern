#include <stdint.h>
#include "mcp2515.h"
#include "mcp2515_regisers.h"
#include "../utilities/printf.h"
#include "../uart/uart.h"
#include <util/delay.h>
void INT0_vect(){
    char nn[] = "SSS\n";
    UART::GetInstance().Write((uint8_t *)nn, 4);
    _delay_ms(10);
    MCP2515 &mcp = MCP2515::GetInstance();
    uint8_t interrupt_flags;
    mcp.ReadFromRegister(MCP_CANINTF, interrupt_flags);
    /*


    if(interrupt_flags & (MCP_TX0IF)){
        // Transmission complete
        mcp.clear_to_send = true;
        // Clear the interrupt
        uint8_t bitmask = (MCP_TX0IF);
        mcp.BitModify(MCP_CANINTF, bitmask, 0x00);
        char nn[] = "ADD\n";
        UART::GetInstance().Write((uint8_t *)nn, 4);
    }

    if(interrupt_flags & (MCP_RX0IF)){
        // We got a new message
        uint8_t data_to_rec[8];
        CAN_MESSAGE rec_mess = CAN_MESSAGE(0,data_to_rec,0);
        mcp.ReadRxFrame(rec_mess);
        //(*mcp.upper_level)(rec_mess);
        // Clear the interrupt
        uint8_t bitmask = (MCP_RX0IF);
        mcp.BitModify(MCP_CANINTF, bitmask, 0x00);

        char nn[] = "KKK\n";
        UART::GetInstance().Write((uint8_t *)nn, 4);
    }*/
}

void MCP2515::Initialize(SPI_N::SPI *spi, uint16_t identifier) {
    this->spi_driver = spi;

    sei();

    GICR |= (1 << INT0);
    MCUCR |= (2<<ISC00);

    // Assuming the SPI is already initialized

    this->Reset();
    // Set interrupts
    this->WriteToRegister(MCP_CANINTE, 0b00000101);

    // Set our identifier
    uint8_t upper_id = (uint8_t) (identifier >> 3);
    uint8_t lower_id = (uint8_t) identifier & 0b111;
    this->WriteToRegister(MCP_RXF0SIDL, (lower_id<<5));
    this->WriteToRegister(MCP_RXF0SIDH, upper_id);

    // Set the mask
    this->WriteToRegister(MCP_RXM0SIDH, 0xFF);
    this->WriteToRegister(MCP_RXM0SIDL, (0xFF << 5));
    printf("Done init!");
}

void MCP2515::RequestToSend() {
    // This is only for buffer 0
    this->spi_driver->WriteByteAndThrowAwayData(0b10000001, 0);
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

void MCP2515::LoadTxFrame(CAN_MESSAGE &message) {
    // TODO: Use sequential writes fully
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
    uint8_t address_data = 0x40 | (buffer_number + 1);
    uint8_t address_sidl = ((0b11 + buffer_number) << 4) + 0b10;
    uint8_t address_dlc = ((0b11 + buffer_number) << 4) + 0b101;


    this->spi_driver->WriteByteAndThrowAwayData(address_sidh, 1);
    this->spi_driver->WriteByteAndThrowAwayData(upper_id, 0);

    this->spi_driver->WriteByteAndThrowAwayData(address_data, 1);
    for(int i = 0; i < message.size; i++){
        if(i == (message.size - 1)){
            this->spi_driver->WriteByteAndThrowAwayData(message.data[i], 0);
        }else{
            this->spi_driver->WriteByteAndThrowAwayData(message.data[i], 1);
        }

    }
    this->WriteToRegister(address_dlc, (message.size & 0x0F));
    this->WriteToRegister(address_sidl, (lower_id<<5));
}

void MCP2515::RxStatus(uint8_t &byte) {
    this->spi_driver->WriteByteAndThrowAwayData(MCP_RX_STATUS,1);
    this->spi_driver->WriteByteAndThrowAwayData(MCP_DC, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(spi_driver->GetAvailableReadBytes() == 0);
    this->spi_driver->ReadByte(byte);
}

void MCP2515::ReadRxFrame(CAN_MESSAGE &message) {
    uint8_t buffer_number = 0;
    uint8_t address_sidh = 0b10010000 | (buffer_number << 2);
    uint8_t address_data = 0b10010010 | (buffer_number << 2);
    uint8_t address_sidl = 0b01100010 | (buffer_number << 4);
    uint8_t address_dlc = 0b01100101 | (buffer_number << 4);

    // Read length
    uint8_t size;
    this->ReadFromRegister(address_dlc, size);
    message.size = (size & 0x0F);

    // Read ID
    uint8_t upper_id;
    uint8_t lower_id;
    this->spi_driver->WriteByteAndThrowAwayData(address_sidh, 1);
    this->spi_driver->WriteByte(MCP_DC, 0);
    while(spi_driver->GetAvailableReadBytes() == 0);
    this->spi_driver->ReadByte(upper_id);

    this->ReadFromRegister(address_sidl, lower_id);
    message.id = (upper_id << 3) + (lower_id >> 5);

    // Read data
    this->spi_driver->WriteByteAndThrowAwayData(address_data, 1);
    for(int i = 0; i < size; i++){
        if(i == (size - 1)){
            this->spi_driver->WriteByte(MCP_DC, 0);
        }else{
            this->spi_driver->WriteByte(MCP_DC, 1);
        }
    }
    while(spi_driver->GetAvailableReadBytes() < size);

    for(int i = 0; i < size; i++){
        this->spi_driver->ReadByte(message.data[i]);
    }


}

void MCP2515::SendMessage(CAN_MESSAGE &message) {
    this->LoadTxFrame(message);
    this->RequestToSend();
}
