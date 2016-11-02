#ifdef __AVR_ATmega2560__

#include "ir_detector.h"

IR_DETECTOR::IR_DETECTOR(uint8_t threshold) : threshold(threshold) {
    this->adc = &ADC_internal::GetInstance();
}

bool IR_DETECTOR::Sample() {

    // Return false if ADC is already in use
    if(!adc->request_sample()){
        return false;
    }

    uint8_t byte;

    // Wait until adc is done reading
    while(!adc->ReadByte(byte)){

    }

    if(byte >= threshold){
        return true;
    } else {
        return false;
    }
}

#endif