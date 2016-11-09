#include "solenoid.h"
#include <util/delay.h>
void Solenoid::Initialize(SPI_N::PIN solenoid_pin) {
    this->pin = solenoid_pin;
    *this->pin.ddr |= (1 << this->pin.pin);
    *this->pin.port &= ~(1 << this->pin.pin);
}

void Solenoid::Pulse() {
    *this->pin.port |= (1 << this->pin.pin);
    _delay_ms(1);
    *this->pin.port &= ~(1 << this->pin.pin);
}
