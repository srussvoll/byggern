#ifdef __AVR_ATmega2560__

#include "ir_detector.h"
#include "lib/utilities/printf.h"

IR_Detector::IR_Detector(){
    this->adc = &ADC_internal::GetInstance();
}

void IR_Detector::Initialize(uint16_t threshold) {
    this->threshold = threshold;
}

bool IR_Detector::Sample() {

    // Return false if ADC is already in use
    if(!adc->RequestSample()){
        return false;
    }

    uint8_t byte;

    // Wait until adc is done reading
    while(!adc->ReadByte(byte)){

    }

    //printf("B = %2x \n", byte);

    return byte <= threshold;
}

#endif