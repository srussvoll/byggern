#include "oled_can.h"


void OLED_SCP::Init(uint8_t width, uint8_t height, SCP &socket) {
    OLED::Init(width, height);
    this->socket = &socket;
}

void OLED_SCP::WriteByteToOLED(volatile uint8_t *address, uint8_t data) {
    uint8_t payload[] = {
            (uint8_t)((uint16_t)address >> 8),
            (uint8_t)(((uint16_t)address & 0xFF)),
            data
    };
    this->socket->Send(4, 0x00, payload);
}

