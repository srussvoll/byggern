#include "oled.h"
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
OLED::OLED(): Stream(1,1){
    *this->oled_command = 0xae; // display off
    *this->oled_command = 0xda; //common pads hardware: alternative
    *this->oled_command = 0x10;
    *this->oled_command = 0x00;
    //*this->oled_command = 0xc8; //common output scan direction:com63~com0
    //*this->oled_command = 0xa8; //multiplex ration mode:63
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
    *this->oled_command = 0xa4; //out follows RAM content
    *this->oled_command = 0xa6; //set normal display
    *this->oled_command = 0xaf; // display on


    // Malloc the Matrix
    this->matrix = (uint8_t **) malloc(8 * sizeof(uint8_t*));
    for(int i = 0; i < 8; i++){
        if ((this->matrix[i] = (uint8_t *) malloc(128 * sizeof(uint8_t))) == NULL){printf("ERROR: Ran out of space on the heap! \n");}
    }
}

void OLED::Clear(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 128; j++){
            this->matrix[i][j] = 0xAA;
        }
    }

    // for(int i = 0; i < 8; i++){
    //     for(int j = 0; j < 128; j++){
    //         printf("%d",this->matrix[i][j]);
    //     }
    //     printf("\n");
    // }
    this->Repaint();
}

void OLED::ClearLine(){
    *this->oled_data = 0x00;
}

void OLED::GoToLine(uint8_t line){
    this->current_line = line;

}

void OLED::Repaint(){
    uint8_t page_address;
    uint8_t column_address;
    for(int i = 0; i<8; i++){
        page_address = 0xB0 + i;
        *this->oled_command = page_address;
        for(int j = 0; j<128; j++){
            column_address = 0x00 + j;
            // Set lower nibble
            *this->oled_command = 0x00 + (column_address & 0xF);

            // Set highern nibble
            *this->oled_command = 0x10 + (column_address>>4);
            //printf("\ni = %d, j = %d, page = %02X, h_n = %02X, l_n = %02X",i,j,page_address,  0x10 + (column_address>>4), 0x00 + (column_address & 0xF));
            *this->oled_data = this->matrix[i][j];
        }
    }
}
