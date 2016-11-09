#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "../lib/utilities/fonts.h"
#include "init.h"
#include "lib/uart/uart.h"
#include "lib/oled_memory/oled_memory.h"
#include "lib/menu/menu.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/mcp2515/mcp2515_regisers.h"
#include "lib/spi/spi.h"
#include "lib/socket/socket.h"
#include "lib/joystick/joystick.h"
#include "state_machine.h"
#include "lib/highscore/highscore.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main(void) {
    //InitializeStateMachine(); // This will call an endless loop.

    Highscore::Highscore hs;
    char name1[] = "Sondre";
    char name2[] = "V";
    char name3[] = "Johan";

    Highscore::Score score1(10, name1, sizeof(name1));
    Highscore::Score score2(30, name2, sizeof(name2));
    Highscore::Score score3(20, name3, sizeof(name3));

    hs.SaveScore(score1);
    hs.SaveScore(score2);
    hs.SaveScore(score3);

    Highscore::Score **hss = hs.GetHighscore(0);

    printf("%4d: %s\n", hss[0]->score, hss[0]->name);
    printf("%4d: %s\n", hss[1]->score, hss[1]->name);
    printf("%4d: %s\n", hss[2]->score, hss[2]->name);



    /*OLED_memory &memory_oled = OLED_memory::GetInstance();
    memory_oled.Init(128, 64);
    OLED &my_oled = (OLED&) memory_oled;

    printf("Done.\n");

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


*/
    /*SOCKET &socket = SOCKET::GetInstance(0);
    printf("Starting \n");

    while(true) {
        while (socket.GetAvailableReadBytes() < 24);
        char s[40];
        socket.Read((uint8_t *) s, 40);
        printf("%s",s);
    }*/

}

#pragma clang diagnostic pop
