#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "../lib/utilities/fonts.h"
#include "init.h"
#include "../lib/uart/uart.h"
#include "../lib/oled/oled.h"
#include "../lib/menu/menu.h"
#include "../lib/mcp2515/mcp2515.h"
#include "../lib/mcp2515/mcp2515_regisers.h"
#include "../lib/spi/spi.h"
#include "../lib/joystick/joystick.h"

uint16_t write_errors       = 0;
uint16_t retrieval_errors   = 0;
// volatile char *ext_ram = (char *) malloc(0x1D00 * sizeof(char)); // Start address for the SRAMuint8_t

void SRAM_test(uint16_t seed) {
    //volatile char *ext_ram = (char *) malloc(0x0500 * sizeof(char)); // Start address for the SRAMuint8_t
    volatile char *ext_ram = (char *) 0x8000; // Start address for the SRAM
    uint16_t ext_ram_size       = 0x1D00;

    printf("Starting SRAM test...\n");



    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)

    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            //printf("Write phase error: ext_ram[%4x] = %02X (should be %02X)\n", &ext_ram[i], retreived_value, some_value);
            write_errors++;
        }
    }

    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            //printf("Retrieval phase error: ext_ram[%4x] = %02X (should be %02X)\n", &ext_ram[i], retreived_value, some_value);
            retrieval_errors++;
        }
    }
    //printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void SPITest();
void OLEDTest();

int main(void) {
    //SPITest();
    OLEDTest();
}

void SPITest(){
    _delay_ms(100);
    SPI_N::SPI &my_spi = SPI_N::SPI::GetInstance();

    SPI_N::PIN ss = SPI_N::PIN(&PORTD, &DDRD, 5);
    SPI_N::PIN ss_a[] = {ss};

    my_spi.Initialize(ss_a, 1, 0,0);
    my_spi.SetDevice(ss);

    MCP2515 &my_mcp = MCP2515::GetInstance();
    my_mcp.Initialize(&my_spi);

    printf("start\n");

    my_mcp.SetLoopback();
    while(1){
        uint8_t byte;
        my_mcp.ReadFromRegister(MCP_CANCTRL,byte);
        _delay_ms(100);
        printf("BYTE = %2x \n", byte);
        break;
    }

};

void OLEDTest(){
    _delay_ms(500);
    printf("\n\n\n----------------------------------------\n");

    OLED &my_oled = OLED::GetInstance();
    my_oled.Init(128,64);
    my_oled.SetFont(Fonts::f8x8, 8, 8);

    Menu::Controller controller(my_oled, 5);

    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);

    char item0[] = "Menylinje 0";
    char item1[] = "Menylinje 1";
    char item2[] = "Menylinje 2";
    char item3[] = "Menylinje 3";
    char item4[] = "Menylinje 4";
    char item5[] = "Menylinje 5";
    char item6[] = "Menylinje 6";
    char item7[] = "Menylinje 7";
    char item8[] = "Submeny 1";
    char item9[] = "Submeny 2";

    //printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);

    Menu::Item *i0 = new Menu::Item(item0, sizeof(item0) - 1);
    Menu::Item *i1 = new Menu::Item(item1, sizeof(item1) - 1);
    Menu::Item *i2 = new Menu::Item(item2, sizeof(item2) - 1);
    Menu::Item *i3 = new Menu::Item(item3, sizeof(item3) - 1);
    Menu::Item *i4 = new Menu::Item(item4, sizeof(item4) - 1);
    Menu::Item *i5 = new Menu::Item(item5, sizeof(item5) - 1);
    Menu::Item *i6 = new Menu::Item(item6, sizeof(item6) - 1);
    Menu::Item *i7 = new Menu::Item(item7, sizeof(item7) - 1);

    Menu::Item *i8 = new Menu::Item(item8, sizeof(item8) - 1);
    Menu::Item *i9 = new Menu::Item(item9, sizeof(item9) - 1);




    Menu::Item* main_items[] = {i0, i1, i2, i3, i4, i5, i6, i7};
    Menu::Item* sub_main_items[] = {i8,i9};

    printf("Sizeof menuitem: %d\n", sizeof(main_items));

    controller.AddMenuItems(main_items, (sizeof(main_items)) / sizeof(main_items[0]));

    controller.ControlGoToItem(4);
    controller.AddMenu(sub_main_items, (sizeof(sub_main_items)) / sizeof(sub_main_items[0]));

    controller.Render();

    for (uint8_t i = 0; i < 4; ++i) {
        _delay_ms(200);
        controller.SelectNext();
    }

    controller.ExecuteItem();
    controller.Render();

    for (uint8_t i = 0; i < 1; ++i) {
        _delay_ms(200);
        controller.SelectNext();
    }

    controller.GoToParent();
    for (uint8_t i = 0; i < 4; ++i) {
        _delay_ms(200);
        controller.SelectNext();
    }

    // ADC and joystick testing
    Joystick& my_joystick = Joystick::GetInstance();
    ADC& adc_x = ADC::GetInstance(ADC_ADDRESS1);
    ADC& adc_y = ADC::GetInstance(ADC_ADDRESS2);

    Quantization levels;
    levels.x_max = 255;
    levels.x_min = 0;
    levels.y_max = 255;
    levels.y_min = 0;
    float percentage = 0.7;
    my_joystick.Init(levels, percentage, &adc_x, &adc_y);

    while(true){
        adc_x.request_sample();
        uint8_t byte;
        while(!adc_x.ReadByte(byte)){
            ;
        }
        printf("tall %d \n", byte);
        if(my_joystick.IsRight()){
            printf("i am right \n");
        }
        _delay_ms(10);
    }


    while (true) {}
}
#pragma clang diagnostic pop


