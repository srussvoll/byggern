#include "scp.h"

SCP::SCP(SOCKET *sockets){
    this->sockets = sockets;
}

void SCP::Send(uint8_t priority, uint8_t command,uint8_t *data, uint8_t length_of_data){
    SOCKET &current_socket = this->sockets[priority];
    uint8_t payload[10] = {command, length_of_data};
    for(int i = 0; i < length_of_data; i++){
        payload[2 + i] = data[i];
    }
    current_socket.Write(payload, 2 + length_of_data);
}