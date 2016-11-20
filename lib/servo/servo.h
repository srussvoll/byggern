#pragma once

#include <avr/io.h>

class Servo {

private:
    uint8_t pin;
    uint16_t min;
    uint16_t max;


public:

    Servo(uint16_t min, uint16_t max);
    void SetAngle(int16_t angle);
};
