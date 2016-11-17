#pragma once

#include <avr/io.h>

class Encoder {
public:
    Encoder();
    int16_t ReadByte();
    void Reset();
};