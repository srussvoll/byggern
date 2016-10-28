#include "dac.h"

DAC::DAC(){
};

bool DAC::WriteAnalogSigal(uint8_t value) {

    // 0x50 gives the address to be written to. The LSB is 0 because of being a write
    uint8_t message[2];
    message[0] = address_and_write_byte;
    message[1] = 0x10;       // The first byte is used for commands.
    message[2] = value;
    i2c.SendData(message, 3);
}

void DAC::Initialize() {
    address_and_write_byte = 0x50;
    //the adress is set as 0x50 by grounding the adress pins
}