#include "adc.h"
//#include <stdio.h>

bool ADC::adc_in_use = false;
volatile uint8_t *ADC::adc_waiting = nullptr;
uint8_t ADC::int_pin = 0;

ADC::ADC(uint16_t address, uint8_t int_pin):address((uint8_t*) address), Stream(1,1){
    sei();
    ADC::int_pin = int_pin;
    GICR |= (1 << INT2);
    EMCUCR &= ~(1 << ISC2);
}

bool ADC::request_sample(){
    if(ADC::adc_in_use){
        // ADC busy
        return false;
    } else{
        // Get the address and write the address to the address
        ADC::adc_waiting = this->address;
        ADC::adc_in_use = true;
        *(this->address) = (uint8_t)(uint16_t) (this->address);
    }
}

void INT2_vect(){
    // Data now valid. Put the data into the stream
    ADC &adc = ADC::GetInstance((uint16_t) ADC::adc_waiting, ADC::int_pin);
    uint8_t data = *adc.address;
    adc.WriteByteToInputStream(data);
    ADC::adc_in_use = false;
}
