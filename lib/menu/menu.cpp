#include <avr/io.h>
#include <math.h>
#include <stdio.h>
#include <util/delay.h>

#include "menu.h"
#include "../utilities/new.h"
#include "../oled/oled.h"

namespace {
    inline int8_t min(int8_t a, int8_t b) {
        return (a < b) ? a : b;
    }

    inline int8_t max(int8_t a, int8_t b) {
        return (a > b) ? a : b;
    }
}

namespace Menu {
    Item::Item(char *label, uint8_t label_length) : label_length(label_length) {
//        printf("Placing the label on the heap.\n");
        this->label = (char *) malloc(label_length * sizeof(char));
        memcpy(this->label, label, label_length);
    }

    void Item::AddAction(void (*fn)()) {
        this->action = (void *) fn;
    }

    Controller::Controller(OLED& oled, uint8_t num_lines) : oled(&oled), num_lines(num_lines) {
        oled.Clear();
        this->root = new Menu;
        this->current_menu_control = this->root;
        this->current_menu_navigate = this->root;
        this->current_item_navigate = this->root->first;
    }

    void Controller::Initialize(OLED &oled, uint8_t num_lines) {
        this->oled = &oled;
        this->num_lines = num_lines;
        oled.Clear();
        this->root = new Menu;
        this->current_menu_control = this->root;
        this->current_menu_navigate = this->root;
        this->current_item_navigate = this->root->first;
    }

    void Controller::ControlGoToRoot() {
        this->current_menu_control = this->root;
        this->current_item_control = nullptr;
    }

    void Controller::ControlGoToItem(uint8_t index) {
        this->current_item_control = this->current_menu_control->first;

        for (uint8_t i = 0; i < index; ++i) {
            this->current_item_control = this->current_item_control->next;
        }
    }

    void Controller::ControlEnterSubMenu() {
        this->current_menu_control = (Menu *) this->current_item_control->action;
        this->current_item_control = nullptr;
    }

    void Controller::ControlEnterSubMenu(uint8_t index) {
        this->ControlGoToItem(index);
        this->current_menu_control = (Menu *) this->current_item_control->action;
        this->current_item_control = nullptr;
    }

    void Controller::ControlGoToParent() {
        this->current_menu_control = this->current_menu_control->parent;
        this->current_item_control = nullptr;
    }

    void Controller::AddMenu(Item **items, uint8_t length) {
        if (this->current_item_control != nullptr) {
            Menu *menu = new Menu();
            menu->parent = this->current_menu_control;
            this->current_item_control->action = menu;
            this->current_item_control->has_sub_menu = true;
        }

        if (length > 0) {
            // Remember current state.
            Menu *current_menu = this->current_menu_control;
            Item *current_item = this->current_item_control;

            this->ControlEnterSubMenu();
            for (uint8_t i = 0; i < length; ++i) {
                this->AddMenuItem(items[i]);
                this->ControlGoToItem(i);
            }

            // Set the state back to the original state.
            this->current_menu_control = current_menu;
            this->current_item_control = current_item;
        }
    }

    void Controller::AddMenuItem(Item *item) {
        if (this->current_item_control == nullptr) {
            item->next = this->current_menu_control->first;
            this->current_menu_control->first = item;
        } else {
            item->next = this->current_item_control->next;
            this->current_item_control->next = item;
        }
    }

    void Controller::AddMenuItem(Item *item, uint8_t index) {
        if (index == 0) {
            item->next = this->current_menu_control->first;
            this->current_menu_control->first = item;
        } else {
            this->ControlGoToItem(index - 1);
            item->next = this->current_item_control->next;
            this->current_item_control->next = item;
        }
    }

    void Controller::GoToMenu(Menu *menu) {
        this->current_menu_navigate = menu;
        this->current_index_navigate = 0;
        this->current_index_selected = 0;
    }

    void Controller::GoToItem(uint8_t index) {
        this->current_item_navigate = this->current_menu_navigate->first;

        for (uint8_t i = 0; i < index; ++i) {
            this->current_item_navigate = this->current_item_navigate->next;
        }
    }

    void Controller::ExecuteItem() {
        this->GoToItem(this->current_index_selected);
        //printf("item to be executed: %s \n", this->current_item_navigate->label);
        if (this->current_item_navigate->has_sub_menu) {
            //printf("I have submeny \n");
            this->GoToMenu((Menu *) this->current_item_navigate->action);
            this->Render();
        }
        else if (this->current_item_navigate->action != nullptr) {
            (*((void (*)()) this->current_item_navigate->action))();
        }
    }

    void Controller::GoToRoot() {
        this->current_menu_navigate = this->root;
        this->current_index_navigate = 0;
        this->current_index_selected = 0;
    }

    void Controller::GoToParent() {
        if (this->current_menu_navigate->parent != nullptr) {
            this->current_menu_navigate = this->current_menu_navigate->parent;
            this->current_index_navigate = 0;
            this->current_index_selected = 0;
        }
        this->Render();
    }

    void Controller::Render() {
        oled->SetNumberOfLines(this->num_lines);
        //_delay_ms(500);

        // Make sure that, if possible, the selected item is not the top or bottom one.
        if (this->current_index_selected <= this->current_index_navigate) {
            this->current_index_navigate = (uint8_t) max((int8_t) 0, (int8_t) this->current_index_selected - (int8_t) 1);
        } else if (this->current_index_selected >= this->current_index_navigate + this->num_lines - 1) {
            this->current_index_navigate = (uint8_t) min((int8_t) this->GetMenuLength(this->current_menu_navigate) - (int8_t) this->num_lines,
                                                         (int8_t) this->current_index_selected - (int8_t) this->num_lines + 2);
        }
        this->GoToItem(this->current_index_navigate);

        uint8_t i = 0;
        uint8_t selected_index_relative = this->current_index_selected - this->current_index_navigate;
        do {
            oled->WriteLine(this->current_item_navigate->label, this->current_item_navigate->label_length, i, 2);

            for(uint8_t j = 0; j < current_item_navigate->label_length; j++){
                //printf("%c", this->current_item_navigate->label[j]);
            }
            //printf("\n");

            if (i == selected_index_relative) {

                // Paints an arrow on the currently selected line
                static uint8_t arrow[1][4] = {{0b00011000, 0b00011000, 0b00111100, 0b00011000}};
                uint8_t *dummy[1] = { arrow[0] };
                uint8_t **arrow_ptr = dummy;
                uint8_t x = 0;
                uint8_t y = oled->GetYCoordinateFromLineNumber(i);
                oled->WriteBitmap(arrow_ptr, 4, 8, x, y, false);
            }
            i++;
        } while (this->GoToNextItem());
        //printf("--- \n");
        oled->Repaint();
    }

    uint8_t Controller::GetMenuLength(Menu *menu) {
        if (menu->first == nullptr) return 0;
        else {
            uint8_t id = 1;
            Item *i = menu->first;
            while((i = i->next) != nullptr) id++;
            return id;
        }
    }

    bool Controller::GoToNextItem() {
        if (this->current_item_navigate->next == nullptr) return false;
        else {
            this->current_item_navigate = this->current_item_navigate->next;
            return true;
        }
    }

    void Controller::SelectNext() {
        //printf("Index selected: %d, Index item: %d\n", this->current_index_selected, this->current_index_navigate);
        if(this->current_index_selected == (this->GetMenuLength(this->current_menu_navigate) - 1)){
            this->current_index_selected = 0;
            this->current_item_navigate = 0;
        } else {
            this->current_index_selected = this->current_index_selected + 1;
        }
        this->Render();
        //printf("Index selected: %d, Index item: %d\n", this->current_index_selected, this->current_index_navigate);
    }

    void Controller::SelectPrevious() {
        if(current_index_selected == 0){
            this->current_index_selected = this->GetMenuLength(this->current_menu_navigate) - 1;
            this->current_index_navigate = this->GetMenuLength(this->current_menu_navigate) - 1;
        } else {
            this->current_index_selected = this->current_index_selected - 1;
        }
        this->Render();
    }

    void Controller::AddMenuItems(Item **items, uint8_t length) {
        Item *current_item = this->current_item_control;

        //printf("len: %d", length);
        for (uint8_t i = 0; i < length; ++i) {
            this->AddMenuItem(items[i]);
            this->ControlGoToItem(i);
        }

        // Set the state back to the original state.
        this->current_item_control = current_item;
    }
}