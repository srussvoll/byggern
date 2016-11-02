#include <stdlib.h>
#include "oled_scp.h"


void OLED_SCP::Init(uint8_t width, uint8_t height, SCP &channel, uint8_t command) {
    this->channel = &channel;
    this->command = command;
    this->last_address = this->oled_data;
    this->bytes_to_send = (uint8_t *) malloc(sizeof(uint8_t) * this->display_width);
    OLED::Init(width, height);
}

void OLED_SCP::WriteByteToOLED(volatile uint8_t *address, uint8_t data) {
    volatile uint8_t *last_address = this->last_address;
    this->last_address = address;
    this->bytes_to_send_index += 1;
    this->bytes_to_send[this->bytes_to_send_index] = data;
    if (this->bytes_to_send_index == this->display_width || last_address != address) {
        uint8_t payload[2 + ] = {
                (uint8_t)((uint16_t)address >> 8),
                (uint8_t)(((uint16_t)address & 0xFF)),
                data
        };

    }

    this->channel->Send(1, this->command, payload, sizeof(payload) / sizeof(payload[0]));
}

