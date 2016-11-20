#pragma once

#include <avr/io.h>

/**
 * \brief A servo driver
 */
class Servo {

private:

    /**
     * Which pin the servo is connected to.
     */
    uint8_t pin;

    /**
     * Minimum time for the servo.
     */
    uint16_t min;

    /**
     * Maximum time for the servo.
     */
    uint16_t max;


public:

    /**
     * Constructor initializing the servo.
     * @param min Minimum time value.
     * @param max Maximum time value.
     */
    Servo(uint16_t min, uint16_t max);

    /**
     * Sets the desired angle in degrees.
     * @param angle Desired angle in degrees.
     */
    void SetAngle(int16_t angle);
};
