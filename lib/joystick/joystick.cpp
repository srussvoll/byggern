#ifdef __AVR_ATmega2560__

#include "joystick.h"
#include "lib/utilities/printf.h"

Joystick::Joystick() {
}

void Joystick::Initialize(Quantization levels, float threshold) {
    this->threshold = threshold;
    this->levels = levels;
    this->x_midpoint = ( levels.x_max - levels.x_min) / 2.0;
    this->y_midpoint = ( levels.y_max - levels.y_min) / 2.0;
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
    if(((float) this->x > this->levels.x_max * this->threshold) && ((float) this->y > this->levels.y_max * this->threshold)){
        return NorthEast;
    } else if(((float) this->x > this->levels.x_max*threshold) && ((float) this->y < this->levels.y_max*((float) 1.0 - this->threshold))){
        return SouthEast;
    } else if(((float) this->x < this->levels.x_max * (1.0 - this->threshold)) && ((float) this->y < this->levels.y_max * ((float) 1.0 - this->threshold))){
        return SouthWest;
    } else if(((float) this->x < this->levels.x_max * (1.0 - this->threshold)) && ((float) this->y > this->levels.y_max * this->threshold)){
        return NorthWest;
    } else if((float) this->x < this->levels.x_max * (1.0 - this->threshold)){
        return West;
    } else if((float) this->x > this->levels.x_max * this->threshold){
        return East;
    } else if((float) this->y > this->levels.y_max * this->threshold){
        if(this->previous_direction != North){
            this->previous_direction = North;
            this->changed_direction = true;
        } else {
            this->changed_direction = false;
        }
        return North;
    } else if((float) this->y < this->levels.y_max * (1.0 - this->threshold)){
        return South;
    } else {
        if(this->previous_direction != None){
            this->previous_direction = None;
            this->changed_direction = true;
        } else {
            this->changed_direction = false;
        }
        return None;
    }
}

float Joystick::GetPercentageY() {
    float value = ((float) this->y - this->y_midpoint)/( this->y_midpoint);
    float percentage = (float) fabs(value);
    return percentage;
}

float Joystick::GetPercentageX() {
    float value = ((float) this->x - this->x_midpoint)/( this->x_midpoint);
    float percentage = (float) fabs(value);
    return percentage;
}
bool Joystick::DirectionChanged() {
    return this->changed_direction;
}

bool Joystick::ButtonIsDown() {
    // Not used so not implemented
}

#endif

