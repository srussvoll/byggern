#ifdef __AVR_ATmega2560__

#include "joystick.h"

Joystick::Joystick() {
}

void Joystick::Init(Quantization levels, float threshold) {
    this->threshold = threshold;
    this->levels = levels;
    this->x_midpoint = ( (float) levels.x_max - (float )levels.x_min) / 2.0;
    this->y_midpoint = ( (float) levels.y_max - (float )levels.y_min) / 2.0;
}


void Joystick::Update(uint8_t x, uint8_t y){
    // Update the x and y value of the joystick
    this->x = x;
    this->y = y;
}

uint8_t Joystick::GetX() {
    return this->x;
}

uint8_t Joystick::GetY() {
    return this->y;
}

Direction Joystick::GetDirection() {
    if(this->y > y_midpoint){
        return North;
    }
}

bool Joystick::ButtonIsDown() {
    // To be implemented
}

#endif