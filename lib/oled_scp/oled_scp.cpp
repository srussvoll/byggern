#include <stdlib.h>
#include "oled_scp.h"


void OLED_SCP::Init(uint8_t width, uint8_t height, SCP &channel, uint8_t command) {
    this->channel = &channel;
    this->command = command;
    this->bytes_to_send = (uint8_t *) malloc(sizeof(uint8_t) * this->display_width);
    OLED::Init(width, height);
}

void OLED_SCP::WriteByteToOLED(volatile uint8_t *address, uint8_t data) {
    this->bytes_to_send[this->bytes_to_send_index] = data;
    this->bytes_to_send_index += 1;
    if (this->bytes_to_send_index == this->display_width || address == this->oled_command) {
        uint8_t payload[130] = {
                (uint8_t)((uint16_t)address >> 8),
                (uint8_t)(((uint16_t)address & 0xFF)),
        };
        memcpy(&payload[2], this->bytes_to_send, this->bytes_to_send_index);
        //printf("len: %d\n", this->bytes_to_send_index);
        this->channel->Send(1, this->command, payload, 2 + this->bytes_to_send_index);
        this->bytes_to_send_index = 0;
    }
}

