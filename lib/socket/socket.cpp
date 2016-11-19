#include <util/delay.h>
#include "socket.h"
#include "../utilities/printf.h"

void SOCKET::Write(uint8_t *string, uint16_t size) {
    uint16_t remaining_data = size;
    do {
        uint8_t can_size;
        uint8_t data[8];

        if(remaining_data >= 8) {

            // More data than one CAN frame allows. Need to split it
            can_size = 8;
            memcpy(data, &string[size-remaining_data],can_size);
            remaining_data -= 8;

        }else {

            // Remaining data fits into one CAN frame
            can_size = remaining_data;
            memcpy(data, &string[size-remaining_data], can_size);

            // Underflow prevention
            if(remaining_data < 8){
                remaining_data = 0;
            }else{
                remaining_data -= 8;
            }
        }
        CAN_MESSAGE message = CAN_MESSAGE(can_size,data,this->id);
        this->can->SendMessage(message);

#ifdef __AVR_ATmega162__
        //_delay_ms(11);
#elif __AVR_ATmega2560__
        _delay_ms(25);
#endif
    } while (remaining_data);
}

void SOCKET::WriteByte(uint8_t byte) {
    uint8_t data[] = {byte};
    CAN_MESSAGE message = CAN_MESSAGE(8,data,this->id);
    this->can->SendMessage(message);
}
