#include "oled.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>

namespace {
    inline uint8_t min(uint8_t a, uint8_t b) {
        return (a < b) ? a : b;
    }
}

OLED::OLED() : Stream(1, 1) {}

void OLED::Init(uint8_t width, uint8_t height){
    this->WriteByteToOLED(this->oled_command, 0xae); // display off
    this->WriteByteToOLED(this->oled_command, 0xa1); //segment remap
    this->WriteByteToOLED(this->oled_command, 0xda); //common pads hardware: alternative
    this->WriteByteToOLED(this->oled_command, 0x12);
    this->WriteByteToOLED(this->oled_command, 0xc8); //common output scan direction:com63~com0
    this->WriteByteToOLED(this->oled_command, 0xa8); //multiplex ration mode:63
    this->WriteByteToOLED(this->oled_command, 0x3f);
    this->WriteByteToOLED(this->oled_command, 0xd5); //display divide ratio/osc. freq. mode
    this->WriteByteToOLED(this->oled_command, 0x80);
    this->WriteByteToOLED(this->oled_command, 0x81); //contrast control
    this->WriteByteToOLED(this->oled_command, 0x50);
    this->WriteByteToOLED(this->oled_command, 0xd9); //set pre-charge period
    this->WriteByteToOLED(this->oled_command, 0x21);
    this->WriteByteToOLED(this->oled_command, 0x20); //Set Memory Addressing Mode
    this->WriteByteToOLED(this->oled_command, 0x00); //Horizontal addressing
    this->WriteByteToOLED(this->oled_command, 0xdb); //VCOM deselect level mode
    this->WriteByteToOLED(this->oled_command, 0x30);
    this->WriteByteToOLED(this->oled_command, 0xad); //master configuration
    this->WriteByteToOLED(this->oled_command, 0x00);
    this->WriteByteToOLED(this->oled_command, 0xa4); //out follows RAM content
    this->WriteByteToOLED(this->oled_command, 0xa6); //set normal display
    this->WriteByteToOLED(this->oled_command, 0xaf); // display on

    this->display_height = height;
    this->display_width = width;


    // Malloc the Matrix
    uint8_t pages = (uint8_t) ceil( (float) height / 8);
    this->number_of_pages = pages;
    this->matrix = (uint8_t**) malloc(pages * sizeof(uint8_t*));
    for(uint8_t i = 0; i < pages; i++){
        if ((this->matrix[i] = (uint8_t *) malloc(width * sizeof(uint8_t))) == NULL){printf("ERROR: Ran out of space on the heap! \n");}
    }

    this->SetNumberOfLines(pages);

    this->Clear();
}

void OLED::GoToLine(uint8_t line){
    this->current_line = line;
}

void OLED::Clear(){
    for(uint8_t i = 0; i < this->number_of_pages; i++){
        for(uint8_t j = 0; j < this->display_width; j++){
            this->matrix[i][j] = 0x00;
        }
    }
}

void OLED::ClearLine(){
    for(uint8_t j = 0; j < 128; j++){
        this->matrix[this->current_line][j] = 0x00;
    }
}

void OLED::WriteByte(uint8_t page, uint8_t column, uint8_t byte){
    this->matrix[page][column] = byte;
}

void OLED::WriteByteArray(uint8_t page, uint8_t column, uint8_t *byte_array, uint8_t length){
    for(uint8_t j = 0; j < length; j++){
        this->matrix[page][column + j] = byte_array[j];
    }
}

void OLED::Repaint(){
    uint8_t page_address;
    uint8_t column_address;
    for(uint8_t i = 0; i < this->number_of_pages; i++){
        page_address = (uint8_t) 0xB0 + i;
        //this->WriteByteToOLED(this->oled_command, page_address);
        for(uint8_t j = 0; j < this->display_width; j++){
            column_address = (uint8_t) 0x00 + j;
            // Set lower nibble
            //this->WriteByteToOLED(this->oled_command, 0x00 + (column_address & 0xF));

            // Set higher nibble
            //this->WriteByteToOLED(this->oled_command, 0x10 + (column_address>>4));
            this->WriteByteToOLED(this->oled_data, this->matrix[i][j]);
            //printf("%x", this->matrix[i][j]);
            //volatile uint8_t u = this->matrix[i][j];
        }
        /*volatile uint8_t *volatile e = this->matrix[i];
        volatile uint8_t *volatile *volatile o = this->matrix;
        volatile uint8_t u = this->matrix[0][0];*/
        //printf("\n");
    }
}

void OLED::SetNumberOfLines(uint8_t number_of_lines){
    this->number_of_lines = number_of_lines;
    this->pixels_per_line = this->display_height/number_of_lines;
}

void OLED::GetBitmapForCharacter(char character, uint8_t* &character_bitmap){
    uint8_t x = (uint8_t) (character - 32);
    character_bitmap = &this->font[this->font_width * x];
}

void OLED::WriteBitmap(uint8_t **pixels, uint8_t bitmap_width, uint8_t bitmap_height, uint8_t x, uint8_t y, bool is_progmem){
    uint8_t columns_to_write = min(bitmap_width,  (x < this->display_width) ? this->display_width - x : 0);
    bitmap_height = min(bitmap_height, this->display_height - y);

    for(uint8_t j = 0; j < columns_to_write; j++){
        // Assume that bitmap_height <= line_height
        uint8_t page_x_starts_at = y/8;
        uint8_t page_x_ends_at = (uint8_t)ceil(((float)y + bitmap_height)/8 - 1);
        // Case 1 : The entire line is on the same page

        uint8_t upper_offset = y - page_x_starts_at*8;
        uint8_t lower_offset = (page_x_ends_at + 1)*8 - (bitmap_height + y);
        //printf("UO: %2X, LO: %2x\n", upper_offset, lower_offset);
        //printf("PLSA : %d, PLEA : %d \n", page_x_starts_at, page_x_ends_at);

        if(page_x_starts_at == page_x_ends_at){
            uint8_t bitmask = (0xFF << upper_offset) & (0xFF >> lower_offset);
            uint8_t col = (is_progmem) ? (pgm_read_byte(&pixels[0][j])) : ( pixels[0][j] );
            this->matrix[page_x_starts_at][x + j] = ((col << upper_offset) & bitmask) | (this->matrix[page_x_starts_at][x + j] & ~(bitmask));
        }else{
            // First page
            uint8_t col = (is_progmem) ? (pgm_read_byte(&pixels[0][j])) : ( pixels[0][j] );
            uint8_t upper_bitmask = (0xFF << upper_offset);
            this->matrix[page_x_starts_at][x + j] = ((col << upper_offset) & upper_bitmask) | (this->matrix[page_x_starts_at][x + j] & ~(upper_bitmask));

            // Between Pages
            for(uint8_t i = 1; i < (page_x_ends_at-page_x_starts_at); ++i){
                col = (is_progmem) ? (pgm_read_byte(&pixels[i - 1][j])) : ( pixels[i - 1][j] );
                this->matrix[page_x_starts_at + i][x + j] = ((col >> (8-upper_offset) & ~upper_bitmask) | (this->matrix[page_x_starts_at + i][x + j] & upper_bitmask));
                col = (is_progmem) ? (pgm_read_byte(&pixels[i][j])) : ( pixels[i][j] );
                this->matrix[page_x_starts_at + i][x + j] = ((col << (upper_offset) & upper_bitmask) | (this->matrix[page_x_starts_at + i][x + j] & ~upper_bitmask));
            }

            // Last page
            uint8_t lower_bitmask = (0xFF >> lower_offset);
            col = (is_progmem) ? (pgm_read_byte(&pixels[bitmap_height/8 - 1][j])) : ( pixels[bitmap_height/8 - 1][j] );
            this->matrix[page_x_ends_at][x + j] = ((col >> lower_offset) & lower_bitmask) | (this->matrix[page_x_ends_at][x + j] & ~(lower_bitmask));
        }
    }

}

void OLED::SetFont(uint8_t *font, uint8_t width, uint8_t height) {
    this->font = font;
    this->font_height = height;
    this->font_width = width;
}

void OLED::WriteLine(char *string, uint8_t length, uint8_t line, uint8_t offset) {
    if (line >= this->number_of_lines) return;

    uint8_t y = line*pixels_per_line;

    uint8_t available_length = min(length, (this->display_width - offset*this->font_width) / this->font_width);

    for(uint8_t i = 0; i < available_length; i++){
        uint8_t *bitmap_row;
        this->GetBitmapForCharacter(string[i], bitmap_row);
        uint8_t **bitmap = &bitmap_row;

        WriteBitmap(bitmap, this->font_width, this->font_height, (offset + i)*this->font_width, y, true);
    }
}

uint8_t OLED::GetYCoordinateFromLineNumber(uint8_t line) {
    return line * this->pixels_per_line;
}