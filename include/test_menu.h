#include "lib/menu/menu.h"

#include <avr/io.h>
#include <util/delay.h>

void test_menu(OLED &oled) {
    Menu::Controller controller(oled, 8);

    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);

    char item0[] = "Menylinje 1";
    char item1[] = "Menylinje 2";
    char item2[] = "Menylinje 3";
    char item3[] = "Menylinje 4";
    char item4[] = "Menylinje 5";
    char item5[] = "Menylinje 6";

    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);

    Menu::Item main_items[] = {
        Menu::Item(item0, sizeof(item0) - 1)
        //Menu::Item(item1, sizeof(item1) - 1),
/*        Menu::Item(item2, sizeof(item2) - 1),
        Menu::Item(item3, sizeof(item3) - 1),
        Menu::Item(item4, sizeof(item4) - 1),
        Menu::Item(item5, sizeof(item5) - 1)*/
    };

    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);

    controller.AddMenuItems(main_items, (sizeof(main_items)) / sizeof(main_items[0]));

    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);

    controller.render();

    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);


/*    _delay_ms(2000);
    controller.SelectNext();
    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);
    _delay_ms(2000);
    controller.SelectNext();
    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);
    _delay_ms(2000);*/
/*    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);
    controller.SelectNext();
    _delay_ms(2000);*/

}