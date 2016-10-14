#pragma once
//#ifdef __AVR_ATMega2560__
#include <avr/io.h>

/**
* @file
* @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
* @version 1.0
*
* A digital-to-analog converter
*/

class DAC{
private:

    /**
    * A constructor that initializes the DAC. Private because of singleton
    */
    DAC();

    /**
    * The maximum voltage value the DAC can output
    */
    uint8_t max;

    /**
    * The minimum voltage value the DAC can output
    */
    uint8_t min;

public:

    static DAC& GetInstance(){
        static DAC instance;
        return instance;
    }

    /**
     * Beacause of singleton - makes sure its not copied etc.
     */
    DAC(const DAC&) = delete;

    /**
    * Beacause of singleton - makes sure its not copied etc.
    */
    void operator=(const DAC&) = delete;

    /**
    * Returns a bool indicating whether or not the value is valid.
    * Writes the value to the DAC
    * @param value The voltage value to be output
    */
    bool WriteAnalogSigal(uint8_t value);
};

//#endif