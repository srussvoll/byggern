#include "oled.h"
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>
OLED::OLED(): Stream(1,1){
}

void OLED::Init(uint8_t width, uint8_t height){
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
        for(int j = 0; j < this->display_width; j++){
            column_address = 0x00 + j;
            // Set lower nibble
            *this->oled_command = 0x00 + (column_address & 0xF);

            // Set higher nibble
            *this->oled_command = 0x10 + (column_address>>4);
            //printf("\ni = %d, j = %d, page = %02X, h_n = %02X, l_n = %02X",i,j,page_address,  0x10 + (column_address>>4), 0x00 + (column_address & 0xF));
            *this->oled_data = this->matrix[i][j];
            //printf("%2X", this->matrix[i][j]);
        }
        //printf("\n");
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
    // for(int i = 0; i < bitmap_width; i++){
    //     // Get the number of pages
    //     for(int j = 0; j < (uint8_t) ceil( (float) bitmap_height / 8); j++) {
    //         // Check that it's not out of bounds
    //         if((x + i < this->display_width) && (y + j < this->display_height)){
    //             // assume bitmap is indexed by row then column
    //             printf("y is %d, x is %d \n", (uint8_t) ceil((float) y / 8),x+i);
    //             this->matrix[(uint8_t) ceil((float) y / 8)+j][x + i] |= bitmap[j][i];
    //         }
    //     }
    // }
    for(int i = 0; i < bitmap_width; i++){
        uint8_t *column = bitmap[i];
    }

}

uint8_t* OLED::GetBitmapForCharacter(uint8_t character, uint8_t font){
    // Get the pointer from PROGMEM (which also points to PROGMEM)
    uint8_t *bitmap = pgm_read_byte(&(font[character-32]))
    // 32 is the ASCII offset
    return bitmap;
}

void OLED::WriteColumn(uint8_t *pixels, uint8_t num_pixels, uint8_t x, uint8_t y){
    // Assume that num_pixels <= line_height
    uint8_t page_x_starts_at = y/8;
    uint8_t page_x_ends_at = ceil(((float)y + num_pixels)/8 - 1);
    // Case 1 : The entire line is on the same page

    uint8_t upper_offset = y - page_x_starts_at*8;
    uint8_t lower_offset = (page_x_ends_at + 1)*8 - (num_pixels + y);
    printf("UO: %2X, LO: %2x\n", upper_offset, lower_offset);
    printf("PLSA : %d, PLEA : %d \n", page_x_starts_at, page_x_ends_at);

    if(page_x_starts_at == page_x_ends_at){
        uint8_t bitmask = (0xFF << upper_offset) & (0xFF >> lower_offset);
        this->matrix[page_x_starts_at][x] = ((pixels[0] << upper_offset) & bitmask) | (this->matrix[page_x_starts_at][x] & ~(bitmask));
    }else{
        // First page
        uint8_t upper_bitmask = (0xFF << upper_offset);
        this->matrix[page_x_starts_at][x] = ((pixels[0] << upper_offset) & upper_bitmask) | (this->matrix[page_x_starts_at][x] & ~(upper_bitmask));

        // Between Pages
        for(int i = 1; i < (page_x_ends_at-page_x_starts_at); ++i){
            this->matrix[page_x_starts_at + i][x] = ((pixels[i - 1] >> (8-upper_offset) & ~upper_bitmask) | (this->matrix[page_x_starts_at + i][x] & upper_bitmask));
            this->matrix[page_x_starts_at + i][x] = ((pixels[i] << (upper_offset) & upper_bitmask) | (this->matrix[page_x_starts_at + i][x] & ~upper_bitmask));
        }

        // Last page
        uint8_t lower_bitmask = (0xFF >> lower_offset);
        this->matrix[page_x_ends_at][x] = ((pixels[num_pixels/8 - 1] >> lower_offset) & lower_bitmask) | (this->matrix[page_x_ends_at][x] & ~(lower_bitmask));
    }
}

void OLED::SetNumberOfLines(uint8_t number_of_lines){
    this->Clear();
    this->number_of_lines = number_of_lines;
    this->pixels_per_line = this->display_height/number_of_lines;
}

void OLED::WriteLine(uint8_t *string, uint8_t length_of_string, uint8_t font, uint8_t font_height, uint8_t font_width, uint8_t offset = 0){
  // Clear the line
  this->ClearLine();
  uint8_t x = offset;
  uint8_t y = this->current_line * this->number_of_lines;
  // For each character, write it to the current line
  for(int i = 0; i < length_of_string; ++i){
    uint8_t *bitmap = this->GetBitmapForCharacter(string[i], font);
    this->WriteBitmap(x,y,bitmap,font_height, font_width, true);
    x += font_width;
  }
}
