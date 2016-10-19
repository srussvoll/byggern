#pragma once
#include "avr/io.h"
#include "../can/can.h"
#include "../stream/stream.h"


class SOCKET : public Stream {
public:

    CAN *can;

    /**
     * A Singleton implementation of this class
     *
     */
    static SOCKET& GetInstance(uint8_t id){
        if(id == 1){
            static SOCKET instance(1);
            return instance;
        }else if (id == 2){
            static SOCKET instance(2);
            return instance;
        }
    }

    static void HandleDataFromLowerLevel(CAN_MESSAGE &message) {
        SOCKET &socket = SOCKET::GetInstance(message.id);
        socket.WriteToInputStream(message.data, message.size);
    }

    SOCKET(const SOCKET&) = delete;

    SOCKET(uint8_t id) : Stream(1,10), id(id){
    }

    void Initialize(CAN *can){
        SOCKET::can = can;
        SOCKET::can->SetUpperLevel(&SOCKET::HandleDataFromLowerLevel);
    }

    void Write(uint8_t *string, uint16_t size);

private:
    uint8_t id;


};
