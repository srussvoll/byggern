#include "can.h"

void CAN::Initialize(MCP2515 *mcp2515_driver) {
    this->mcp2515_driver = mcp2515_driver;
}

void CAN::SendMessage(uint16_t id, uint8_t *message, uint8_t message_length) {
    // We need the first 11 bits of ID
    uint8_t lower_id = (uint8_t) (id & 0x0F);
    uint8_t upper_id = (uint8_t) (id >> 8);
}

CAN::CAN(): Stream(128,128) {

}
