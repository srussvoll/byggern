#pragma once
#include "avr/io.h"
#include "../can/can.h"
class SOCKET : public Stream {
public:
    static CAN *can;

    /**
     * A Singleton implementation of this class
     *
     */
    static SOCKET& GetInstance(uint8_t id){
        if(id == 1){
            static SOCKET instance;
            return instance;
        }else if (id == 2){
            static SOCKET instance;
            return instance;
        }
    }

    static void HandleDataFromLowerLevel(CAN_MESSAGE &message) {
        SOCKET &socket = SOCKET::GetInstance(message.id);
        socket.Write(message.data, message.size);
    }

    SOCKET(const SOCKET&) = delete;

    SOCKET(CAN *can) : Stream(10,10){}

    void Initialize(CAN *can){
        SOCKET::can = can;
        SOCKET::can->SetUpperLevel(&SOCKET::HandleDataFromLowerLevel);
    }

};
