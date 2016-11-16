#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "encoder.h"

Encoder::Encoder() {
    DDRH |= (1 << DDH5);
    DDRH |= (1 << DDH3);
    DDRH |= (1 << DDH6);
    DDRK |= 0x00;

    this->Reset();
}

void Encoder::Reset() {
    PORTH &= ~(1 << PH6);
    _delay_us(20);
    PORTH |=  (1 << PH6);
}

int16_t Encoder::ReadByte() {
    PORTH &= ~(1 << PH5);

    PORTH &= ~(1 << PH3);
    _delay_us(20);
    __asm("NOP");
    int16_t byte = PINK << 8;

    PORTH |= (1 << PH3);
    _delay_us(20);
    __asm("NOP");
    byte |= PINK;

    PORTH |= (1 << PH5);

    return byte;
}