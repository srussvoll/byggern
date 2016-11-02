#include <math.h>

#include "dac.h"

DAC::DAC(){
    i2c.Initialize(0x0C);
    this->dac_number = 1;
};

bool DAC::WriteAnalogSignalRaw(uint8_t value) {
    if((value <= this->max) && (value >= this->min)){
        // The second byte is used for commands.
        uint8_t message[3];
        message[0] = this->address_and_write_byte;
        message[1] = this->GetCommandForDAC();
        message[2] = value;
        i2c.SendData(message, 3);
        return true;
    } else {
        return false;
    }
}

void DAC::Reset(){
    uint8_t message[2];
    message[0] = this->address_and_write_byte;
    message[1] = 0x10;
    i2c.SendData(message, 2);
}

void DAC::SetDAC(uint8_t dac_number){
    this->dac_number = dac_number;
}

uint8_t DAC::GetCommandForDAC() {
    switch(dac_number){
        case 1:
            return 0x00;
        case 2:
            return 0x01;
        case 3:
            return 0x02;
        case 4:
            return 0x03;
        default:
            return 0x00;
    }
}

void DAC::Initialize(uint8_t min, uint8_t max, uint8_t i2c_address) {
    this->max = max;
    this->min = min;
    this->address_and_write_byte = i2c_address;
    //the address is set as 0x50 by grounding the address pins
}

bool DAC::WriteAnalogSignalPercentage(float percentage) {
    if((percentage > 1.0) || (percentage < 0.0)){
        return false;
    }
    float value = percentage * (float) this->max;

    uint8_t value_uint8 = (uint8_t) floor(value);
    uint8_t message[3];
    message[0] = this->address_and_write_byte;
    message[1] = this->GetCommandForDAC();
    message[2] = value_uint8;
    i2c.SendData(message,3);
    return true;
}