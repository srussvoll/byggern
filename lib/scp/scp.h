#pragma once

#include "../socket/socket.h"

/**
 * \brief SCP (Simple Command Protocol) is an application layer protocol designed to send commands.
 *
 * The frame is built up by first one byte identifying the command, the second byte identifying the length of the data (n),
 * and the last n bytes is the data.
 *
 * This class is using Socket as its link layer implementation.
 */
class SCP {

private:

    /**
     * An array with sockets
     */
    Socket* *sockets;

    /**
     * The number of sockets
     */
    uint8_t number_of_sockets;


public:

    /**
     * Initializer
     * @param sockets An array of sockets that you want to use
     * @param number_of_sockets Number of sockets in the sockets array
     */
    SCP(Socket* *sockets, uint8_t number_of_sockets);

    /**
     * Sends some data with priority and command
     * @param priority The socket you want to send this data into. This number refers to the array you passed in the initializer
     * @param command The command you are sending. See the commands.h for a list of commands
     * @param data An array containing the data you are sending
     * @param length_of_data The length of the data array
     */
    void Send(uint8_t priority, uint8_t command, uint8_t *data, uint8_t length_of_data);

    /**
     * Polls if any new messages have been received
     * @param data An array you want the data to be put into
     * @param The length of the data will be put into this variable
     */
    bool Receive(uint8_t &command, uint8_t *data, uint8_t &length_of_data);
};