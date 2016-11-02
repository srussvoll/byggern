#pragma once
#include "../socket/socket.h"
/**
 * SCP (Simple Command Protocol) is an application layer protocol designed to send commands. The frame is built up by
 * first one byte, identifying the command, the second byte identifying the length of the data (n),
 * and the last n bytes is the data.
 */
class SCP{
public:
    SCP(SOCKET* *sockets, uint8_t number_of_sockets);

    void Send(uint8_t priority, uint8_t command, uint8_t *data, uint8_t length_of_data);
    bool Receive(uint8_t &command, uint8_t *data, uint8_t &length_of_data);
private:
    SOCKET* *sockets;
    uint8_t number_of_sockets;
};