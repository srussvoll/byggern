#ifdef __AVR_ATmega2560__

#include "Solenoid.h"
#include <util/delay.h>
#include <avr/io.h>

void Solenoid::Initialize() {
    DDRF |= (1 << DDF1);
    PORTF &= ~(1 << PORTF1);
}

void Solenoid::Pulse() {
    PORTF |= (1 << PORTF1);
    // Wait 80ms before turning it of again.
    _delay_ms(80);
    PORTF &= ~(1 << PORTF1);
}
#endif __ATMega2560__