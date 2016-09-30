#include "oled.h"
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

OLED::OLED(): Stream(1,1){
}

void OLED::Init(uint8_t width, uint8_t height){
    printf("Init oled\n");
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
    *this->oled_command = 0x02; //Page addressing
    *this->oled_command = 0xdb; //VCOM deselect level mode
    *this->oled_command = 0x30;
    *this->oled_command = 0xad; //master configuration
    *this->oled_command = 0x00;
    *this->oled_command = 0xa4; //out follows RAM content
    *this->oled_command = 0xa6; //set normal display
    *this->oled_command = 0xaf; // display on

    this->display_height = height;
    this->display_width = width;


    // Malloc the Matrix
    uint8_t pages = (uint8_t) ceil( (float) height / 8);
    this->number_of_pages = pages;
    this->matrix = (uint8_t **) malloc(pages * sizeof(uint8_t*));
    for(int i = 0; i < pages; i++){
        if ((this->matrix[i] = (uint8_t *) malloc(width * sizeof(uint8_t))) == NULL){printf("ERROR: Ran out of space on the heap! \n");}
    }


    this->SetNumberOfLines(pages);
    printf("%d\n", pages);
}

void OLED::Clear(){
    printf("Clearing, %d \n", this->number_of_pages);
    for(int i = 0; i < this->number_of_pages; i++){
        for(int j = 0; j < this->display_width; j++){
            this->matrix[i][j] = 0x00;
        }
    }
}

void OLED::ClearLine(){
    for(int j = 0; j < 128; j++){
        this->matrix[this->current_line][j] = 0x00;
    }
}

void OLED::GoToLine(uint8_t line){
    this->current_line = line;
}

void OLED::Repaint(){
    uint8_t page_address;
    uint8_t column_address;
    for(int i = 0; i < this->number_of_pages; i++){
        page_address = 0xB0 + i;
        *this->oled_command = page_address;
        for(int j = 0; j < this->display_height; j++){
            column_address = 0x00 + j;
            // Set lower nibble
            *this->oled_command = 0x00 + (column_address & 0xF);

            // Set higher nibble
            *this->oled_command = 0x10 + (column_address>>4);
            //printf("\ni = %d, j = %d, page = %02X, h_n = %02X, l_n = %02X",i,j,page_address,  0x10 + (column_address>>4), 0x00 + (column_address & 0xF));
            *this->oled_data = this->matrix[i][j];
        }
    }
}

void OLED::WriteByte(uint8_t page, uint8_t column, uint8_t byte){
    this->matrix[page][column] = byte;
}

void OLED::WriteByteArray(uint8_t page, uint8_t column, uint8_t *byte_array, uint8_t length){
    for(int j = 0; j < length; j++){
        this->matrix[page][column + j] = byte_array[j];
    }
}

void OLED::WriteBitmap(uint8_t x, uint8_t y, uint8_t** bitmap, uint8_t bitmap_height, uint8_t bitmap_width){
    for(int i = 0; i < bitmap_width; i++){
        for(int j = 0; j < (uint8_t) ceil( (float) bitmap_height / 8); j++) {
            if((x + i < this->display_width) && (y + j < this->display_height)){
                // assume bitmap is indexed by row then column
                printf("y is %d, x is %d \n", (uint8_t) ceil((float) y / 8),x+i);
                this->matrix[(uint8_t) ceil((float) y / 8)+j][x + i] |= bitmap[j][i];
            }
        }
    }
}

uint8_t* OLED::GetBitmap(uint8_t character, uint8_t **font){
    // 32 is the ASCII offset
    return font[character-32];
}

void OLED::WriteColumnToPages(uint8_t *pixels, uint8_t line, uint8_t column){
    // Assume that num_pixels <= line_height
    uint8_t page_line_starts_at = (line)*this->pixels_per_line/8;
    uint8_t page_line_ends_at = (line+1)*this->pixels_per_line/8;
    // Case 1 : The entire line is on the same page
    if(page_line_starts_at == page_line_ends_at){
        uint8_t upper_offset = line*this->pixels_per_line - page_line_starts_at*8;
        uint8_t lower_offset = (page_line_ends_at + 1)*8 - (line + 1)*this->pixels_per_line;
        uint8_t bitmask = (0xFF >> upper_offset) & (0xFF << lower_offset);
        this->matrix[page_line_starts_at][column] = (pixels[0] & bitmask) | (this->matrix[page_line_starts_at][column] & ~(bitmask));
    }
}

void OLED::SetNumberOfLines(uint8_t number_of_lines){
    this->Clear();
    this->number_of_lines = number_of_lines;
    this->pixels_per_line = this->display_height/number_of_lines;
}
