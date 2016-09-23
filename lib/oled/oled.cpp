#include "oled.h"
#include <stdio.h>
#include <util/delay.h>
OLED::OLED(): Stream(1,64){
    printf("Init oled");
    *this->oled_command = 0xae; // display off
    *this->oled_command = 0xa1; //segment remap
    *this->oled_command = 0xda; //common pads hardware: alternative
    *this->oled_command = 0x12;
    *this->oled_command = 0xc8; //common output scan direction:com63~com0
    *this->oled_command = 0xa8; //multiplex ration mode:63
    *this->oled_command = 0x3f;
    *this->oled_command = 0xd5; //display divide ratio/osc. freq. mode
    *this->oled_command = 0x80;
    *this->oled_command = 0x81; //contrast control
    *this->oled_command = 0x50;
    *this->oled_command = 0xd9; //set pre-charge period
    *this->oled_command = 0x21;
    *this->oled_command = 0x20; //Set Memory Addressing Mode
    *this->oled_command = 0x02;
    *this->oled_command = 0xdb; //VCOM deselect level mode
    *this->oled_command = 0x30;
    *this->oled_command = 0xad; //master configuration
    *this->oled_command = 0x00;
    *this->oled_command = 0xa4; //out follows RAM content
    *this->oled_command = 0xa6; //set normal display
    *this->oled_command = 0xaf; // display on

    // Bind callback here
    while(true) {
        *this->oled_data = 0xAA;
        _delay_us(500);
    }
}

void OLED::Clear(){

}

void OLED::ClearLine(){

}

void OLED::GoToLine(uint8_t line){
    this->current_line = line;
}

void OLED::EventOutputBufferNotEmpty(){
    while(this->GetOutputBufferLength() > 0){
        // Write bytes to the oled data address

    }
}
