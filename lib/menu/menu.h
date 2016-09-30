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
        Menu *root;

        Menu *current_menu_control = nullptr;
        Item *current_item_control = nullptr;

        Menu    *current_menu_navigate = nullptr;
        uint8_t  current_index_navigate = 0;
    public:
        Controller();

        void FindRoot();
        void FindItem(uint8_t index);
        void EnterSubMenu();
        void EnterSubMenu(uint8_t index);
        void FindParent();

        void AddMenu(Item *items, uint8_t length);
        void AddMenuItem(Item &item);
        void AddMenuItem(Item &item, uint8_t index);

        void GoTo(Menu *menu);
        void GoToRoot();
        void GoToParent();

        void ExecuteItem(Menu *menu, uint8_t index);

        void render();
    };
}