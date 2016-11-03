#include <util/delay.h>
#include "socket.h"
#include "../utilities/printf.h"

void SOCKET::Write(uint8_t *string, uint16_t size) {
    uint16_t data_left = size;
    while(data_left > 0){
        uint8_t can_size;
        uint8_t data[8];
        if(data_left >= 8){
            can_size = 8;
            memcpy(data, &string[size-data_left],can_size);
            data_left -= 8;
        }else{
            can_size = data_left;
            memcpy(data, &string[size-data_left], can_size);
            // Underflow prevention
            if(data_left < 8){
                data_left = 0;
            }else{
                data_left -= 8;
            }
        }
        CAN_MESSAGE message = CAN_MESSAGE(can_size,data,this->id);
        this->can->SendMessage(message);
        _delay_ms(11);
    }
}

void SOCKET::WriteByte(uint8_t byte) {
    uint8_t data[] = {byte};
    CAN_MESSAGE message = CAN_MESSAGE(8,data,this->id);
    this->can->SendMessage(message);
}
