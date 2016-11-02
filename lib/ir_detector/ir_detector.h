#ifdef __AVR_ATmega2560__

#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <lib/adc_internal/adc_internal.h>

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * An infrared detector to detect if there is an object blocking the path of the sensor
 */
class IR_DETECTOR {

private:

    /**
     * A constructor that initializes the UART to a certain size
     */
    IR_DETECTOR(uint8_t threshold);

    /**
     * Threshold for the IR sensor
     */
    uint8_t threshold;

    ADC_internal* adc;

public:
    /**
    * A Singleton implementation of this class
    */
    static IR_DETECTOR& GetInstance(){
        // Initialize the IR_DETECTOR with a certain threshold
        static IR_DETECTOR instance(150);
        return instance;
    }

    /**
     * Samples the IR Detector
     * @return bool Returns whether or not there is an object blocking the IR sensor
     */
    bool Sample();

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    IR_DETECTOR(const IR_DETECTOR&) = delete;

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    void operator=(const IR_DETECTOR&) = delete;
};

#endif

