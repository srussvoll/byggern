#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>

#include "../stream/stream.h"
#include "../uart/uart.h"

ISR(ADC_vect);

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * A class for using the internal analog-to-digital converter on the AVR
 */
class ADC_internal : public Stream {

private:

    /**
     * A flag indicating if the ADC is currently used
     */
    volatile bool adc_in_use;

    /**
     * Constructor for ADC class. It is private due to singleton implementation
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
     * The interrupt vector for ADC done. Friend function because it needs access to the private members
     */
    friend void ADC_vect();

    /**
     * Request the ADC to get a sample. This sample can be read from the buffer using ReadByte(uint8_t& byte);
     */
    bool RequestSample();

};