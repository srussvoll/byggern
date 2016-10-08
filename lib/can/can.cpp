#include "can.h"

void CAN::Initialize(MCP2515 *mcp2515_driver) {
    this->mcp2515_driver = mcp2515_driver;
}
