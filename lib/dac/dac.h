#pragma once
#include <avr/io.h>
#include "lib/i2c/i2c.h"

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
    *
    */
    /* FIXME: SOOOOOONDREEEEE!!!!!!!!!!!! Dette er ikke greit... */
    I2C& i2c = I2C::GetInstance();

    /**
    * The maximum voltage value the DAC can output
    */
    uint8_t max;

    /**
    * The minimum voltage value the DAC can output
    */
    uint8_t min;

    uint8_t address_and_write_byte;

public:

    static DAC& GetInstance(){
        static DAC instance;
        return instance;
    }

    /**
     * Initializes the dac by giving it an address (the one used by I2C to access it)
     * In our project it is already grounded, so the adress is set as 0x50
     */
    void Initialize();

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    DAC(const DAC&) = delete;

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    void operator=(const DAC&) = delete;

    /**
     * Returns a bool indicating whether or not the value is valid.
     * Writes the value to the DAC
     * @param value The voltage value to be output
     */
    bool WriteAnalogSigal(uint8_t value);
};