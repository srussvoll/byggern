#include "adc_internal.h"

ADC_internal::ADC_internal(): Stream(1,1){
    sei();

    // Initialize ADC
    this->adc_in_use = false;

    // Set reference voltage for ADC to AREF
    ADMUX &= ~( 1 << REFS0 );
    ADMUX &= ~( 1 << REFS1 );

    // Select channel for internal adc. Default is ADC0
    ADMUX &= ~( 1 << MUX0 );
    ADMUX &= ~( 1 << MUX1 );
    ADMUX &= ~( 1 << MUX2 );

    // Enable the ADC
    ADCSRA |= ( 1 << ADEN );

    // Enable interrupts
    ADCSRA |= ( 1 << ADIE );
}

bool ADC_internal::request_sample(){
    if(this->adc_in_use){
        // ADC_internal busy
        return false;
    } else{
        this->adc_in_use = true;

        // Start Conversion
        ADCSRA |= ( 1 << ADSC );

        return true;
    }
}

void ADC_vect(){
    // Data now valid. Put the data into the stream
    ADC_internal &adc = ADC_internal::GetInstance();
    UART& uart = UART::GetInstance();

    //Get the data from the conversion
    uint8_t data = ADCL;

    adc.WriteByteToInputStream(data);
    uart.WriteByte(data);

    adc.adc_in_use = false;
}

