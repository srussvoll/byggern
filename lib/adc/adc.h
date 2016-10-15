#pragma once

#ifdef __AVR_ATmega162__

// x direction on the joystick
#define ADC_ADDRESS1 (uint8_t *)0x2004

// y direction on the joystick
#define ADC_ADDRESS2 (uint8_t *)0x2005
#define ADC_INT      INT2_vect

#include "../stream/stream.h"
#include <avr/io.h>
#include <avr/interrupt.h>


ISR(ADC_INT);

class ADC : public Stream {
private:

    volatile uint8_t *address;

    /**
    * Constructor for ADC class. Singleton implementation
    * @param address The address the ADC is located at. Channel on the adc is determined by the LSB.
    * We use 0x2004 for channel 1 and 0x2005 for channel 2
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
        } else {
            static ADC instance(ADC_ADDRESS2);
            return instance;
        }
    }

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    ADC(const ADC&) = delete;

    /**
    * Because of singleton - makes sure its not copied etc.
    */
    void operator=(const ADC&) = delete;

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