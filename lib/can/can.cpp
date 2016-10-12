#include "can.h"
#include "../mcp2515/mcp2515_regisers.h"
void CAN::Initialize(MCP2515 *mcp2515_driver) {
    this->mcp2515_driver = mcp2515_driver;
}

void CAN::SendMessage(uint16_t id, uint8_t *message, uint8_t message_length) {
}

void CAN::SendByte(uint16_t id, uint8_t byte){

}

CAN::CAN(): Stream(128,128){}
