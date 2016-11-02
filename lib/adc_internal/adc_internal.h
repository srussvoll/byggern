#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../stream/stream.h"
#include "../uart/uart.h"

ISR(ADC_vect);

class ADC_internal : public Stream {
private:

    volatile uint8_t *address;

    /**
    * Constructor for ADC class. Singleton implementation
    */
    ADC_internal();


public:

    /**
     * A Singleton implementation of this class. See ADC(uint16_t address) for more information.
     */
    static ADC_internal& GetInstance() {
            static ADC_internal instance;
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
    friend void ADC_vect();

    /**
    * Request the ADC to get a sample. This sample can be read from the buffer using ReadByte(uint8_t& byte);
    */
    bool request_sample();

    /**
    * A flag indicating if the ADC is currently used
    */
    volatile static bool adc_in_use;


};
