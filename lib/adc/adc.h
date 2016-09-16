#pragma once

#include "../stream/stream.h"
#include <avr/io.h>
#include <avr/interrupt.h>


ISR(INT2_vect);

class ADC : public Stream {
private:
    volatile uint8_t *address;
    static uint8_t int_pin;
    /**
    * Constructor for ADC class. Singleton implementation
    * @param address The address the ADC is located at. Channel on the adc is determined by the LSB.
    * We use 0x1404 for channel 1 and 0x1405 for channel 2
    */
    ADC(uint16_t address, uint8_t int_pin);


public:

    /**
     * A Singleton implementation of this class. See ADC(uint16_t address) for more information.
     *
     */
    static ADC& GetInstance(uint16_t adc_number, uint8_t int_pin) {
        if(adc_number == 0x1404) {
            static ADC instance(0x1404, int_pin);
            return instance;
        } else {
            static ADC instance(0x1405, int_pin);
            return instance;
        }
    }

    /**
     * Beacause of singleton - makes sure its not copied etc.
     */
    ADC(const ADC&) = delete;

    /**
    * Beacause of singleton - makes sure its not copied etc.
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

    static bool adc_in_use;

    volatile static uint8_t *adc_waiting;

};
