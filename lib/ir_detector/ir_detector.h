#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <lib/adc_internal/adc_internal.h>

/**
 * \brief A IR detector for detecting game over
 *
 * An infrared detector to detect if there is an object blocking the path of the sensor.
 * The main function of interest is the sample function.
 * As usual, the implementation of the IRDetector is chip and system dependent
 */
class IRDetector {

private:

    /**
     * A constructor that initializes the UART to a certain size
     */
    IRDetector();

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
    static IRDetector& GetInstance(){
        // Initialize the IR_DETECTOR with a certain threshold
        static IRDetector instance;
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
    IRDetector(const IRDetector&) = delete;

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    void operator=(const IRDetector&) = delete;
};