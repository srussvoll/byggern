#pragma once

#include <avr/io.h>

/**
 * \brief A servo driver
 */
class Servo {

private:

    /**
     * Which pin the servo is located at
     */
    uint8_t pin;

    /**
     * Minimum value
     */
    uint16_t min;

    /**
     * Maximum value
     */
    uint16_t max;


public:

    /**
     * Constructor
     * @param min Minimum value
     * @param max Maximum value
     */
    Servo(uint16_t min, uint16_t max);

    /**
     * Sets the desired angle
     * @param angle Desired angle
     */
    void SetAngle(int16_t angle);
};
