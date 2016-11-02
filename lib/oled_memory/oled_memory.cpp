#include "oled_memory.h"


void OLED_memory::Init(uint8_t width, uint8_t height) {
    OLED::Init(width, height);
}

void OLED_memory::WriteByteToOLED(volatile uint8_t *address, uint8_t data) {
    *address = data;
}

