#ifdef __AVR_ATmega2560__

#include "adc_internal.h"

#include <util/delay.h>

#include "lib/utilities/printf.h"

ADC_internal::ADC_internal(): Stream(1, 1) {
    sei();

    // Enable the ADC
    ADCSRA |= (1 << ADEN);

    // Set reference voltage for ADC to AREF
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);

    // Set the prescaler
    ADCSRA |= (1 << ADPS0);
    ADCSRA |= (1 << ADPS1);
    ADCSRA |= (1 << ADPS2);

    // Select channel for internal adc. Default is ADC0
    ADMUX &= ~(1 << MUX0);
    ADMUX &= ~(1 << MUX1);
    ADMUX &= ~(1 << MUX2);

    // Enable interrupts
    ADCSRA |= (1 << ADIE);
}

bool ADC_internal::RequestSample(){
    if(this->adc_in_use){
        // ADC_internal busy
        return false;
    } else {
        this->adc_in_use = true;

        // Start Conversion
        ADCSRA |= ( 1 << ADSC );

        return true;
    }
}

void ADC_vect(){
    // Data now valid. Put the data into the stream.
    ADC_internal &adc = ADC_internal::GetInstance();
    UART& uart = UART::GetInstance();

    //Get the data from the conversion and write it to the stream
    uint8_t data = ADCL;
    uint8_t data2 = ADCH;

    adc.WriteByteToInputStream(data);

    adc.adc_in_use = false;
}

#endif