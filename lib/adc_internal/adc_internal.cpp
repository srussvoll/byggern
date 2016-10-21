#include "adc_internal.h"

#ifdef __AVR_ATmega162__

volatile bool ADC_internal::adc_in_use = false;

volatile uint8_t *ADC_internal::adc_waiting = nullptr;

ADC_internal::ADC_internal(uint8_t *address) : address(address), Stream(1,1){
    sei();
    GICR |= (1 << INT2);
    EMCUCR &= ~(1 << ISC2);
}

bool ADC_internal::request_sample(){
    if(ADC_internal::adc_in_use){
        // ADC_internal busy
        return false;
    } else{
        ADC_internal::adc_in_use = true;
        ADC_internal::adc_waiting = this->address;

        // Enables the chip select
        *(this->address) = (uint8_t)(uint16_t) (this->address);
        return true;
    }
}

void ADC_internal_INT(){
    // Data now valid. Put the data into the stream
    ADC_internal &adc = ADC_internal::GetInstance((uint8_t *) ADC_internal::adc_waiting);
    uint8_t data = *adc.address;
    adc.WriteByteToInputStream(data);
    ADC_internal::adc_in_use = false;
}

#endif
