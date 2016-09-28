#pragma once

#include <avr/io.h>

namespace Menu {
    struct Item {
        char *label = nullptr;
        void *action = nullptr;
        bool has_sub_menu = false;
        Item *next = nullptr;
    };

    struct Menu {
        Menu *parent = nullptr;
        Item *first = nullptr;
    };

    class Controller {
    private:
        Menu *current_menu_control = nullptr;
        Item *current_item_control = nullptr;

        Menu    *current_menu_navigate = nullptr;
        uint8_t  current_index_navigate = 0;
    public:
        Controller();

        void FindTop();
        bool Find(uint8_t index);
        bool FindSubMenu();
        bool FindSubMenu(uint8_t index);
        bool FindParent();

        void AddMenu(Item *items, uint8_t length);
        void AddMenuItem(Item item);

        bool GoTo(Menu *menu, uint8_t index);
        bool ExecuteAction(void *action);
    };
}