#ifdef __AVR_ATmega2560__

#include "ir_detector.h"

IRDetector::IRDetector(){
    this->adc = &ADC_internal::GetInstance();
}

void IRDetector::Initialize(uint16_t threshold) {
    this->threshold = threshold;
}

bool IRDetector::Sample() {

    // Return false if ADC is already in use
    if(!adc->RequestSample()){
        return false;
    }

    uint8_t byte;

    // Wait until adc is done reading
    while(!adc->ReadByte(byte)){
    }

    return byte <= threshold;
}

#endif