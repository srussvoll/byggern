#include "servo.h"
#include <avr/io.h>

Servo::Servo(uint16_t min, uint16_t max) : min(min), max(max) {
    // Set pin to output
    DDRB |= (1 << PINB7);
    // Set non inverting mode, set fast PWM
    TCCR0A |= (3 << COM0A0) | (7 << WGM00);
    // Set frequency
    TCNT0 |= (4 << CS00);

    OCR0A = 0x88;
}

void Servo::SetAngle(int16_t angle) {

}