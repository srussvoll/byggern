#pragma once
#include "avr/io.h"
#include "../can/can.h"
#include "../stream/stream.h"
#include <stdio.h>

/**
 * \brief A socket which defines the communication in a CAN channel.
 *
 * It uses an ID to identify which channel it is listening and sending on. This interface consists of two sockets
 * , although it is flexible to increase the amount of sockets. See GetInstance() for more information regarding extending the
 * number of sockets. The lower the socket identifier, the higher the priority.
 */
class SOCKET : public Stream {
public:

    CAN *can;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
    /**
     * A Singleton implementation of this class. In order to have support for more than two sockets, this if else statement
     * has to bee extended to the amount of sockets you wish to support.
     * @param id The socket ID
     */
    static SOCKET& GetInstance(uint8_t id = 0){
        if(id == 0){
            static SOCKET instance(0);
            return instance;
        }else if (id == 1){
            static SOCKET instance(1);
            return instance;
        }
    }
#pragma clang diagnostic pop

    /**
     * Called when a new CAN_MESSAGE from the can controller has arrived. Puts the message into the stream
     * @param message The received CAN_MESSAGE
     */
    static void HandleDataFromLowerLevel(CAN_MESSAGE &message) {
        SOCKET &socket = SOCKET::GetInstance(message.id);
        socket.WriteToInputStream(message.data, message.size);
    }

    /**
     * Because of singleton, make sure it is not deleted
     */
    SOCKET(const SOCKET&) = delete;

    /**
     * The initializer. Not used by end user due to singleton
     * @param id The socket ID
     */
    SOCKET(uint8_t id) : Stream(255, 1), id(id){
    }

    /**
     * Initializes the socket.
     * @param can A can controller which implements the interface defined in the can class in this project
     * @param target_id The target node id
     */
    void Initialize(CAN *can){
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
};
