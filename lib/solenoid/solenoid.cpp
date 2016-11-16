#ifdef __AVR_ATmega2560__

#include "solenoid.h"
#include <util/delay.h>
#include <avr/io.h>

void Solenoid::Initialize() {
    DDRF |= (1 << DDF1);
    PORTF &= ~(1 << PORTF1);
}

void Solenoid::Pulse() {
    PORTF |= (1 << PORTF1);
    _delay_ms(50);
    PORTF &= ~(1 << PORTF1);
    //_delay_ms(30);
}
#endif __ATMega2560__