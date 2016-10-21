#pragma once

// Make the syntax highlighter highlight the preferred device code.
// FIXME: (finish) Remove this.
#if DEBUG
#define __AVR_ATmega2560__
#endif

#ifdef __AVR_ATmega2560__
#include "../stream/stream.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define ADC_INT ADC_vect

ISR(ADC_INT);

class ADC_internal : public Stream {
private:

    volatile uint8_t *address;

    /**
    * Constructor for ADC class. Singleton implementation
    * @param address The address the ADC is located at. Channel on the adc is determined by the LSB.
    * We use 0x2004 for channel 1 and 0x2005 for channel 2
    */
    ADC_internal(uint8_t *address);


public:

    /**
     * A Singleton implementation of this class. See ADC(uint16_t address) for more information.
     */
    static ADC_internal& GetInstance(uint8_t *address) {
            static ADC_internal instance(nullptr);
            return instance;
    }

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    ADC_internal(const ADC_internal&) = delete;

    /**
    * Because of singleton - makes sure its not copied etc.
    */
    void operator=(const ADC_internal&) = delete;

    /**
    * The interrupt vector for ADC done
    */

    friend void INT2_vect();

    /**
    * Request the ADC to get a sample. This sample can be read from the buffer using ReadByte(uint8_t& byte);
    */

    bool request_sample();


    /**
    * A flag indicating if the ADC is currently used
    */
    volatile static bool adc_in_use;

    volatile static uint8_t *adc_waiting;

};

#endif