#pragma once
#include "../socket/socket.h"
/**
 * SCP (Simple Command Protocol) is an application layer protocol designed to send commands. The frame is built up by
 * first one byte, identifying the command, second bit, identifying the length of the data (n),
 * and lastly n bits of data.
 */
class SCP{
public:
    SCP(SOCKET *sockets);

    void Send(uint8_t priority, uint8_t command,uint8_t *data, uint8_t length_of_data);
private:
    SOCKET *sockets;
};