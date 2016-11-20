#ifdef __AVR_ATmega162__

#include "adc.h"
#include "lib/uart/uart.h"

volatile bool ADC::adc_in_use = false;

volatile uint8_t *ADC::adc_waiting = nullptr;

ADC::ADC(uint8_t *address) : address(address), Stream(1,1){
    sei();
    GICR |= (1 << INT2);
    EMCUCR &= ~(1 << ISC2);
}

bool ADC::request_sample(){
    if(ADC::adc_in_use){
        // ADC busy
        return false;
    } else{
        ADC::adc_in_use = true;
        ADC::adc_waiting = this->address;

        // Enables the chip select
        *(this->address) = (uint8_t)(uint16_t) (this->address);
        return true;
    }
}

void ADC_INT(){
    // Data now valid. Put the data into the stream
    ADC &adc = ADC::GetInstance((uint8_t *) ADC::adc_waiting);
    uint8_t data = *adc.address;
    adc.WriteByteToInputStream(data);
    ADC::adc_in_use = false;
}

#endif
