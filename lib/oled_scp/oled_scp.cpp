#include "oled_scp.h"


void OLED_SCP::Init(uint8_t width, uint8_t height, SCP &channel, uint8_t command) {
    this->channel = &channel;
    this->command = command;
    OLED::Init(width, height);
}

void OLED_SCP::WriteByteToOLED(volatile uint8_t *address, uint8_t data) {
    uint8_t payload[] = {
            (uint8_t)((uint16_t)address >> 8),
            (uint8_t)(((uint16_t)address & 0xFF)),
            data
    };

    this->channel->Send(1, this->command, payload, sizeof(payload) / sizeof(payload[0]));
}

