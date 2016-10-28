#pragma once
#include "avr/io.h"
#include "../can/can.h"
#include "../stream/stream.h"
#include <stdio.h>

/**
 * A socket which defines the communcation between two nodes. It uses an ID to identifiy itself, and a target_id to
 * identify the target node. Please note that this is not in coherence with the CAN standard, which implements ID as
 * an identifier of the type of message, and not the target node.
 */
class SOCKET : public Stream {
public:

    CAN *can;

    /**
     * A Singleton implementation of this class
     * @param id The socket ID
     */
    static SOCKET& GetInstance(uint8_t id = 1){
        if(id == 1){
            static SOCKET instance(1);
            return instance;
        }else if (id == 2){
            static SOCKET instance(2);
            return instance;
        }
    }
    /**
     * Called when a new CAN_MESSAGE from the can controller has arrived. Puts the message into the stream
     * @param message The recieved CAN_MESSAGE
     */
    static void HandleDataFromLowerLevel(CAN_MESSAGE &message) {
        printf("Handler from lower \n");
        SOCKET &socket = SOCKET::GetInstance(message.id);
        socket.WriteToInputStream(message.data, message.size);
    }

    SOCKET(const SOCKET&) = delete;

    SOCKET(uint8_t id) : Stream(128,1), id(id){
    }

    /**
     * Initializes the socket.
     * @param can A can controller which implements the interface defined in the can class in this project
     * @param target_id The target node id
     */
    void Initialize(CAN *can, uint8_t target_id){
        this->target_id = target_id;
        SOCKET::can = can;
        SOCKET::can->SetUpperLevel(&SOCKET::HandleDataFromLowerLevel);
    }

    /**
     * Creates the required number of can messages and sends the data to the targeted id
     * @param string Data to be sent
     * @param size Size of the data
     */
    void Write(uint8_t *string, uint16_t size);

    /**
     * Creates a can message and sends the byte to the targeted id
     * @param byte
     */
    void WriteByte(uint8_t byte);

private:
    /**
     * This sockets identifier
     */
    uint8_t id;
    /**
     * Target sockets identifier
     */
    uint8_t target_id;


};