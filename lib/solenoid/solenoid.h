#pragma once
#include "lib/spi/spi.h"
class Solenoid{
public:
    /**
     * A Singleton implementation of this class
     *
     */
    static Solenoid& GetInstance(){
        static Solenoid instance;
        return instance;
    }

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    Solenoid(const Solenoid&) = delete;

    Solenoid(){};

    void Initialize(SPI_N::PIN solenoid_pin);

    void Pulse(uint8_t pulse_width_ms);

private:
    SPI_N::PIN pin;
};