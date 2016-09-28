#pragma once

#include <avr/io.h>

namespace Menu {
    struct Item {
        char *label = nullptr;
        void *action = nullptr;
        bool has_sub_menu = false;
        MenuItem *next = nullptr;
    };

    struct Menu {
        Menu *parent = nullptr;
        Item *first = nullptr;
    };

    class Controller {
    private:
        Menu     *current_menu_control = Menu;
        MenuItem *current_item_control = nullptr;

        Menu     *current_menu_navigate = nullptr;
        uint8_t   current_index_navigate = 0;
    public:
        Menu();

        void FindTop();
        bool Find(uint8_t index);
        bool FindSubMenu();
        bool FindSubMenu(uint8_t index);
        bool FindParent();

        void AddMenu(MenuItem *items, uint8_t length);
        void AddMenuItem(MenuItem item);

        bool GoTo(Menu *menu, uint8_t index);
        bool ExecuteAction(void *action);
    };
}