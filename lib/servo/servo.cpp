// Make the syntax highlighter highlight the preferred device code.
// FIXME: (finish) Remove this.
#if DEBUG
#define __AVR_ATmega2560__
#endif

#ifdef __AVR_ATmega2560__

#include "servo.h"
#include <avr/io.h>

namespace {
    uint16_t mod(int16_t a, int16_t b) {
        if (b < 0) return mod(a, -b);
        int16_t ret = a % b;
        if (ret < 0) ret += b;
        return (uint16_t) ret;
    }
}

Servo::Servo(uint16_t min, uint16_t max) : min(min), max(max) {
    // Set pin to output
    DDRB |= (1 << DDB5);
    // Set non inverting mode
    TCCR1A |= (2 << COM1A0);
    // Set fast PWM
    TCCR1B |= (1 << WGM13) | (1 << WGM12);
    TCCR1A |= (1 << WGM11) | (0 << WGM10);
    // Set frequency
    TCCR1B |= (3 << CS10);
    ICR1 |= 5000;

    this->SetAngle(0);
}

void Servo::SetAngle(int16_t angle) {
    float angleMultiplier = ::mod(angle + 180, 360) / 360.0;
    float counterMultiplier = (this->min + angleMultiplier * (this->max - this->min)) / 20000;
    OCR1A = (uint16_t) (counterMultiplier * 5000);
}

#endif