#ifdef __AVR_ATmega2560__

#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>

#include "lib/dac/dac.h"

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * A motor controller
 */
class Motor{
private:

    /**
     * Default constructor - private because of singleton
     */
    Motor();

    /**
     * The dac used to output the voltage to the motor
     */
    DAC* dac;

    /**
     * The direction of the motor
     */
    uint8_t direction;

public:

    /**
     * A Singleton implementation of this class
     */
    static Motor& GetInstance(){
        // Initialize the Motor
        static Motor instance;
        return instance;
    }

    /**
     * Initialize the motor
     */
    void Initialize();

    /**
     * Sets the speed of the motor
     */
    void SetSpeed(uint8_t speed);

    /**
     * Starts the motor
     */
    void Start();

    /**
     * Stops the motor
     */
    void Stop();

    /**
     * Sets the direction of the motor
     */
    void SetDirection(uint8_t direction);

    /**
     * Drive the motor
     * @param percentage The percentage of the maximum percentage the motor should be driven on
     */
    void Drive(float percentage);

    /**
     * Change the motor direction
     */
    void ChangeDirection();

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    Motor(const Motor&) = delete;

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    void operator=(const Motor&) = delete;
};

#endif