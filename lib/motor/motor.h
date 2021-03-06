#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#include "lib/dac/dac.h"

/**
 * \brief A motor controller for driving a motor through a motor box
 *
 * A motor controller for driving a motor through a motor box.
 * It has the basic functionally expected of a motor (go left, go right, drive, change direction etc.).
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
     * Starts the motor
     */
    void Start();

    /**
     * Stops the motor
     */
    void Stop();

    /**
     * Sets the motor to the left
     */
    void GoLeft();

    /**
     * Sets the motor to the right
     */
    void GoRight();

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