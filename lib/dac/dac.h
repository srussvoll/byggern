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
    * The I2C for communication with the DAC
    */
    I2C& i2c = I2C::GetInstance();

    /**
    * The maximum voltage value the DAC can output
    */
    uint8_t max;

    /**
    * The minimum voltage value the DAC can output
    */
    uint8_t min;

    /**
     * The address of the DAC on the I2C bus
     */
    uint8_t address_and_write_byte;

    /**
     * The DAC to be written to. In range 1-4. Default value is 1
     */
    uint8_t dac_number;

public:

    static DAC& GetInstance(){
        static DAC instance;
        return instance;
    }

    /**
     * Initializes the dac by giving it an address (the one used by I2C to access it)
     * In our project it is already grounded, so the adress is set as 0x50
     * @param min The minimum value the DAC can output
     * @param max The maximum value the DAC can output
     * @param i2c_address The address of the DAC on the I2C bus
     */
    void Initialize(uint8_t min, uint8_t max, uint8_t i2c_address);

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    DAC(const DAC&) = delete;

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    void operator=(const DAC&) = delete;

    /**
     * Resets the DAC. Sets all the voltage outputs to zero.
     */
    void Reset();

    /**
     * Sets the DAC number to output the voltage signal
     * @param The DAC output pin to be put a signal on
     */
    void SetDAC(uint8_t dac_number);

    /**
     * Selects the command corresponding to the dac_number
     */
    uint8_t GetCommandForDAC();

    /**
     * Returns a bool indicating whether or not the value is valid.
     * Writes the value to the DAC
     * @param value The voltage value to be output
     * @return A boolean indicating if the output is valid
     */
    bool WriteAnalogSignalRaw(uint8_t value);

    /**
     * Writes a voltage to the output in a certain percentage of the maximum output
     * @param percentage The percentage of the maximum output
     * @return A boolean indicating if the output is valid
     */
    bool WriteAnalogSignalPercentage(float percentage);
};