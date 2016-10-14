#pragma once
#ifdef __AVR_ATMega162__
#include "../adc/adc.h"

struct Quantization{
    uint8_t x_max;
    uint8_t x_min;
    uint8_t y_max;
    uint8_t y_min;
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
    * A reference to the adc channel for the x direction
    */
    ADC* adc_x;

    /**
    * The midpoint for the x value
    */
    uint8_t x_midpoint;

    /**
    * The midpoint for the y value
    */
    uint8_t y_midpoint;

    /**
    * A reference to the adc channel for the y direction
    */
    ADC* adc_y;

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
    * @param adc_x A pointer to the adc object representating the channel for the x direction
    * @param adc_y A pointer to the adc object representating the channel for the y direction
    */
    void Init(Quantization levels, float threshold, ADC* adc_x, ADC* adc_y);

    /**
    * Because of singleton - makes sure its not copied etc.
    */
    Joystick(const Joystick&) = delete;

    /**
    * Returns true if the joystick controller is to the right
    */
    bool IsRight();

    /**
    * Returns true if the joystick controller is to the left
    */
    bool IsLeft();

    /**
    *Returns true if the joystick controller is down
    */
    bool IsDown();

    /**
    * Returns true if the joystick controller is up
    */
    bool IsUp();

    /**
    * Returns true if the button is pushed down
    */
    bool ButtonIsDown();

};
#endif
