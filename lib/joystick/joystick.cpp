#include "joystick.h"

Joystick::Joystick() {
}

void Joystick::Init(Quantization levels, float threshold, ADC* adc_x, ADC* adc_y) {
    this->adc_x = adc_x;
    this->adc_x = adc_y;
    this->threshold = threshold;
    this->levels = levels;
}

bool Joystick::IsRight() {
    adc_x->request_sample();
    uint8_t x_value;
    while(!adc_x->ReadByte(x_value)){
        ;
    }
    return x_value > uint8_t (threshold*levels.x_max);
}

bool Joystick::IsLeft() {

}

bool Joystick::IsDown() {

}

bool Joystick::IsUp() {

}