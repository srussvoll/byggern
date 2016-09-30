#pragma once

#include <avr/io.h>
#include "../oled/oled.h"

namespace Menu {
    struct Item {
        char *label = nullptr;
        uint8_t label_length;
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
        OLED *oled = nullptr;
        uint8_t num_lines;

        Menu *root;

        Menu *current_menu_control = nullptr;
        Item *current_item_control = nullptr;

        Menu    *current_menu_navigate = nullptr;
        Item    *current_item_navigate = nullptr;
        uint8_t  current_index_navigate = 0;
        uint8_t  current_index_selected = 0;

        uint8_t GetMenuLength(Menu *menu);
    public:
        Controller(OLED &oled, uint8_t num_lines);

        void ControlGoToRoot();
        void ControlGoToItem(uint8_t index);
        void ControlEnterSubMenu();
        void ControlEnterSubMenu(uint8_t index);
        void ControlGoToParent();

        void AddMenu(Item *items, uint8_t length);
        void AddMenuItem(Item &item);
        void AddMenuItem(Item &item, uint8_t index);

        void GoToMenu(Menu *menu);
        void GoToItem(uint8_t index);
        void GoToRoot();
        void GoToParent();
        bool GoToNext();

        void SelectNext();
        void SelectPrevious();


        void ExecuteItem();

        void render();
    };
}