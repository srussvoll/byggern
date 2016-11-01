#include "scp.h"

SCP::SCP(SOCKET* *sockets, uint8_t number_of_sockets) : sockets(sockets), number_of_sockets(number_of_sockets) {}

bool SCP::Receive(uint8_t &command, uint8_t *data, uint8_t &length_of_data) {
    // Return the highest priority command.
    for (int i = 0; i < this->number_of_sockets; ++i) {
        if (this->sockets[i]->GetAvailableReadBytes() > 0) {
            while (this->sockets[i]->GetAvailableReadBytes() < 2)
            this->sockets[i]->ReadByte(command);
            this->sockets[i]->ReadByte(length_of_data);

            while (this->sockets[i]->GetAvailableReadBytes() < length_of_data);
            this->sockets[i]->Read(data, length_of_data);

            return true;
        }
    }

    return false;
}

void SCP::Send(uint8_t priority, uint8_t command,uint8_t *data, uint8_t length_of_data){
    SOCKET *current_socket = this->sockets[priority];
    uint8_t payload[10] = {command, length_of_data};
    for(int i = 0; i < length_of_data; i++){
        payload[2 + i] = data[i];
    }
    printf("%d\n", payload[0]);
    current_socket->Write(payload, 2 + length_of_data);
}