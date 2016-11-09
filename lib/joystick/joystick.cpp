#ifdef __AVR_ATmega2560__

#include "joystick.h"
Joystick::Joystick() {
}

void Joystick::Init(Quantization levels, float threshold) {
    this->threshold = threshold;
    this->levels = levels;

    // Init PORTE2 as write for button
    DDRE &= ~(1 << DDE2);
}

bool Joystick::IsRight(uint8_t x, uint8_t y) {

}

bool Joystick::IsLeft(uint8_t x, uint8_t y) {

}

bool Joystick::IsDown(uint8_t x, uint8_t y) {

}

bool Joystick::IsUp(uint8_t x, uint8_t y) {

}

bool Joystick::IsRight() {

}

bool Joystick::IsLeft() {

}

bool Joystick::IsDown() {

}

bool Joystick::IsUp() {

}

void Joystick::Update(uint8_t x, uint8_t y){
    // Update the x and y value of the joystick
    this->x = x;
    this->y = y;
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


#endif
