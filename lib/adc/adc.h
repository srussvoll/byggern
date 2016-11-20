#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../stream/stream.h"

// x direction on the joystick
#define ADC_ADDRESS1 (uint8_t *)0x2004

// y direction on the joystick
#define ADC_ADDRESS2 (uint8_t *)0x2005

// position slider
#define ADC_ADDRESS3 (uint8_t *)0x2006

// angle slider
#define ADC_ADDRESS4 (uint8_t *)0x2007


#define ADC_INT      INT2_vect
ISR(ADC_INT);


/**
 * \brief ADC library for the ADC0844.
 *
 * This library implements several of the ADC channels through the external memory interface.
 */
class ADC : public Stream {

private:

    /**
     * This is the address of the ADC.
     * This detemines which ADC mode is used (LSB).
     */
    volatile uint8_t *address;

    /**
     * A flag indicating if the ADC is currently in use.
     */
    volatile static bool adc_in_use;

    /**
     * A flag indicating which ADC is currently in use.
     */
    volatile static uint8_t *adc_waiting;

    /**
     * Constructor for ADC class. Singleton implementation.
     *
     * @param address The address the ADC is located at.
     */
    ADC(uint8_t *address);


public:

    /**
     * A Singleton implementation of this class. See ADC(uint16_t address) for more information.
     */
    static ADC& GetInstance(uint8_t *address) {
        if(address == ADC_ADDRESS1) {
            static ADC instance(ADC_ADDRESS1);
            return instance;
        } else if (address == ADC_ADDRESS2) {
            static ADC instance(ADC_ADDRESS2);
            return instance;
        } else if (address == ADC_ADDRESS3) {
            static ADC instance(ADC_ADDRESS3);
            return instance;
        } else {
            static ADC instance(ADC_ADDRESS4);
            return instance;
        }
    }

    /**
     * Singleton specifics.
     */
    ADC(const ADC&) = delete;

    /**
     * Singleton specifics.
     */
    void operator=(const ADC&) = delete;

    /**
     * The interrupt vector for ADC done
     */
    friend void ADC_INT();

    /**
     * Request the ADC to get a sample. This sample can be read from the buffer using ReadByte(uint8_t& byte);
     */
    bool request_sample();
};