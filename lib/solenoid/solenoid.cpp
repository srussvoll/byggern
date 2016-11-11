#include "solenoid.h"
#include <util/delay.h>
#include <avr/io.h>

void Solenoid::Initialize() {
    DDRK |= (1 << DDK0);
    PORTK &= ~(1 << PORTK0);
}

void Solenoid::Pulse() {
    PORTK |= (1 << PORTK0);
    _delay_ms(50);
    PORTK &= ~(1 << PORTK0);
    //_delay_ms(30);
}
