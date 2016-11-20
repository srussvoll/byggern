#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <lib/adc_internal/adc_internal.h>

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * An infrared detector to detect if there is an object blocking the path of the sensor.
 * The main function of interest is the sample function
 */
class IR_Detector {

private:

    /**
     * A constructor that initializes the UART to a certain size
     */
    IR_Detector();

    /**
     * Threshold for the IR sensor
     */
    uint16_t threshold;

    /**
     * The ADC used to sample the voltage.
     */
    ADC_internal* adc;

public:
    /**
     * A Singleton implementation of this class
     */
    static IR_Detector& GetInstance(){
        // Initialize the IR_DETECTOR with a certain threshold
        static IR_Detector instance;
        return instance;
    }

    /**
     * Used to initialize the singleton object
     * @param threshold The wanted threshold value for the sensor
     */
    void Initialize(uint16_t threshold);

    /**
     * Samples the IR Detector
     * @return bool Returns whether or not there is an object blocking the IR sensor
     */
    bool Sample();

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    IR_Detector(const IR_Detector&) = delete;

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    void operator=(const IR_Detector&) = delete;
};