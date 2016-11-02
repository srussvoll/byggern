#ifdef __AVR_ATmega2560__

#include "motor.h"

Motor::Motor(){
    this->dac = &DAC::GetInstance();
}

void Motor::ChangeDirection() {
    if(PORTH & (1 << PORTH1)){
        // It is zero. Set to one
        PORTH |= (1 << PORTH1);
    } else {
        // It is one. Set to zero
        PORTH &= ~(1 << PORTH1);
    }
}

void Motor::Initialize() {

    // Initializes the dac used in the motor controller
    DAC& dac = DAC::GetInstance();
    dac.Initialize(0, 0xFF, 0x50);
    dac.Reset();

    // Use channel
    dac.SetDAC(4);

    // Enable the motor registers for writing
    DDRH |= (1 << DDH4);
    DDRH |= (1 << DDH1);

}

void Motor::Start() {

    // Turns on the motor
    PORTH |= (1 << PORTH4);
}

void Motor::Stop(){

    // Turn of the motor
    PORTH &= ~(1 << PORTH4);
}

void Motor::SetSpeed(uint8_t speed) {
    this->speed = speed;
}

void Motor::Drive(){

    //TODO To use speed to generate input voltage

    // Send an voltage to the motor through the dac
    this->dac->WriteAnalogSignalPercentage(0.5);
}

#endif