#ifdef __AVR_ATmega162__
#include "joystick.h"
#include "direction.h"
Joystick::Joystick() {
}

void Joystick::Init(Quantization levels, float threshold, ADC* adc_x, ADC* adc_y) {
    this->adc_x = adc_x;
    this->adc_x = adc_y;
    this->threshold = threshold;
    this->levels = levels;

    // Init PORTE2 as write for button
    DDRE &= ~(1 << DDE2);
}

bool Joystick::IsRight() {
    adc_x->request_sample();
    uint8_t x_value;
    while(!adc_x->ReadByte(x_value)){
        ;
    }
    return x_value > (uint8_t) (threshold*(levels.x_max - levels.x_min));
}

bool Joystick::IsLeft() {
    adc_x->request_sample();
    uint8_t x_value;
    while(!adc_x->ReadByte(x_value)){
        ;
    }

    return x_value < (uint8_t) ((levels.x_max - levels.x_min) * (1.0 - threshold)) ;
}

bool Joystick::IsDown() {
    adc_y->request_sample();
    uint8_t y_value;
    while(!adc_y->ReadByte((y_value))){
        ;
    }

    return y_value < (uint8_t) ((levels.y_max - levels.y_min) * (1.0 - threshold)) ;
}

bool Joystick::IsUp() {
    adc_y->request_sample();
    uint8_t y_value;
    while(!adc_y->ReadByte((y_value))){
        ;
    }
    return y_value < (uint8_t) (threshold*(levels.y_max - levels.y_min));
}

bool Joystick::ButtonIsDown() {
    return (((PORTE >> PORTE2) & 1) == 0);
}

uint8_t Joystick::XValue(){
    adc_y->request_sample();
    uint8_t y_value;
    while(!adc_y->ReadByte((y_value))){
        ;
    }

    return y_value;
}

uint8_t Joystick::YValue() {
    adc_y->request_sample();
    uint8_t y_value;
    while(!adc_y->ReadByte((y_value))){
        ;
    }
    return y_value;
}

uint8_t Joystick::GetDirection() {
    
}

#endif
