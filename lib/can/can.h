#pragma once
#include "../mcp2515/mcp2515.h"
#include "../stream/stream.h"

class CAN: Stream{
public:
    /**
    * A Singleton implementation of this class
    *
    */
    static CAN& GetInstance(){
        static CAN instance;
        return instance;
    }

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    CAN(const CAN&) = delete;

    void Initialize(MCP2515 *mcp2515_driver);

    void SendMessage(uint16_t id, uint8_t *message, uint8_t message_length);

    void SendByte(uint16_t id, uint8_t byte);



private:
    CAN();
    MCP2515 *mcp2515_driver;
};