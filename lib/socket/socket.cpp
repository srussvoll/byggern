#include "socket.h"
#include "../utilities/printf.h"

void SOCKET::Write(uint8_t *string, uint16_t size) {
    uint16_t data_left = size;
    while(data_left > 0){
        // Generate message
        uint8_t can_size;
        uint8_t data[64];
        if(data_left >= 64){
            can_size = 64;
            memcpy(data, &string[size-data_left],can_size);
            data_left -= 64;
        }else{
            can_size = data_left;
            memcpy(data, &string[size-data_left], can_size);
            data_left -= 64;
        }
        CAN_MESSAGE message = CAN_MESSAGE(can_size,data,this->id);
        this->can->SendMessage(message);
    }
}

void SOCKET::WriteByte(uint8_t byte) {
    uint8_t string[] = {byte};
    this->Write(string,8);
}
