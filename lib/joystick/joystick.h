#pragma once
#ifdef __AVR_ATmega2560__

#include <avr/io.h>
#include "../adc/adc.h"

struct Quantization{
    uint8_t x_max;
    uint8_t x_min;
    uint8_t y_max;
    uint8_t y_min;
    Quantization(){};
    Quantization(uint8_t x_max, uint8_t x_min, uint8_t y_max, uint8_t y_min): x_max(x_max), x_min(x_min), y_max(y_max),y_min(y_min){}
};

/**
 * @brief A general joystick controller
 * The joystick controller expects a quantizationlevel struct as input. This is to indicate the max and min y and x direction of the joystick.
 * Example: If x goes from 0 to 255, it expects input struct with x_max = 255, x_min = 0. If y goes from 0 to 255, it expects the same for y.
 * It also expect a uint8_t threshold, given in decimal. This threshold is used for detecting direction
 * This must be initialized through the Init function
 */
class Joystick {

private:

    /**
     * A quantization struct containing the quantization levels.
     */
    Quantization levels;

    /**
     * A threshold used to calculate whether or not the joystick is in a given direction
     */
    float threshold;

    /**
     * The midpoint for the x value
     */
    uint8_t x_midpoint;

    /**
     * The midpoint for the y value
     */
    uint8_t y_midpoint;

    /**
     * The x value
     */
    uint8_t x;

    /**
     * The y value
     */
    uint8_t y;

    /**
    * The constructor. Private because of singleton
    */
    Joystick();

public:
    static Joystick& GetInstance(){
        static Joystick instance;
        return instance;
    }

    /**
    * @param levels A quantization struct containing the quantization levels
    * @param threshold_percentage The threshold used to calculate directions
    */
    void Init(Quantization levels, float threshold);

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    Joystick(const Joystick&) = delete;

    /**
     * Returns true if the joystick controller is to the right
     * @param x The x value of the joystick through the adc
     * @param y The y value of the joystick through the adc
     * @return A bool indicating the status
     */
    bool IsRight(uint8_t x, uint8_t y);

    /**
     * Returns if the joystick controller is to the right. Uses the internal x and y values
     * @return A bool indicating the status
     */
    bool IsRight();

    /**
     * Returns true if the joystick controller is to the left
     * @param x The x value of the joystick through the adc
     * @param y The y value of the joystick through the adc
     * @return A bool indicating the status
     */
    bool IsLeft(uint8_t x, uint8_t y);

    /**
     * Returns if the joystick controller is to the right. Uses the internal x and y values
     * @return A bool indicating the status
     */
    bool IsLeft();

    /**
     *Returns true if the joystick controller is down
     * @param x The x value of the joystick through the adc
     * @param y The y value of the joystick through the adc
     * @return A bool indicating the status
     */
    bool IsDown(uint8_t x, uint8_t y);

    /**
     * Returns if the joystick controller is to the right. Uses the internal x and y values
     * @return A bool indicating the status
     */
    bool IsDown();

    /**
     * Returns true if the joystick controller is up
     * @param x The x value of the joystick through the adc
     * @param y The y value of the joystick through the adc
     * @return A bool indicating the status
     */
    bool IsUp(uint8_t x, uint8_t y);

    /**
     * Returns if the joystick controller is to the right. Uses the internal x and y values
     * @return A bool indicating the status
     */
    bool IsUp();

    /**
     * Updated the value of the joystick through the dac (and through the can bus)
     * @param x The x value of the joystick through the adc
     * @param y The y value of the joystick through the adc
     */
    void Update(uint8_t x, uint8_t y);

    /**
     * Returns true if the button is pushed down
     */
    bool ButtonIsDown();

    /**
     * Returns 0 for undefined, 1 for left and 2 for right
     */
    uint8_t XValue();

    /**
     * Returns 0 for undefined, 1 for up and 2 for down
     */
    uint8_t YValue();

};
#endif
