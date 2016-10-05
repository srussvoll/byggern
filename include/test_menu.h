#include "lib/menu/menu.h"

void test_menu() {
    Menu::Controller controller;

    char item0[] = "Menylinje 1";
    char item1[] = "Menylinje 2";
    char item2[] = "Menylinje 3";
    char item3[] = "Menylinje 4";
    char item4[] = "Menylinje 5";
    char item5[] = "Menylinje 6";

    Menu::Item main_items[] = {
            Menu::Item {item0, sizeof(item0)},
            Menu::Item {item1, sizeof(item1)},
            Menu::Item {item2, sizeof(item2)},
            Menu::Item {item3, sizeof(item3)},
            Menu::Item {item4, sizeof(item4)},
            Menu::Item {item5, sizeof(item5)},
    };

    controller.AddMenuItems(main_items, 6);

    controller.render();
}