#include "menu.h"
#include <avr/io.h>
#include <math.h>
#include "../utilities/new.h"
#include "../oled/oled.h"

namespace {
    inline uint8_t min(uint8_t a, uint8_t b) {
        return (a < b) ? a : b;
    }

    inline uint8_t max(uint8_t a, uint8_t b) {
        return (a > b) ? a : b;
    }
}

namespace Menu {
    Controller::Controller(OLED& oled, uint8_t num_lines) : oled(&oled), num_lines(num_lines) {
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

        for (int i = 0; i < index; ++i) {
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

    void Controller::AddMenu(Item *items, uint8_t length) {

    }

    void Controller::AddMenuItem(Item &item) {
        Item *i = new Item(item);
        if (this->current_item_control == nullptr) {
            i->next = this->current_menu_control->first;
            this->current_menu_control->first = i;
            this->current_item_control = i;

        } else {
            i->next = this->current_item_control->next;
            this->current_item_control->next = i;
        }
    }

    void Controller::AddMenuItem(Item &item, uint8_t index) {
        Item *i = new Item(item);
        if (index == 0) {
            i->next = this->current_menu_control->first;
            this->current_menu_control->first = i;

        } else {
            this->ControlGoToItem(index - 1);
            i->next = this->current_item_control->next;
            this->current_item_control->next = i;
        }
    }

    void Controller::GoToMenu(Menu *menu) {
        this->current_menu_navigate = menu;
        this->current_index_navigate = 0;
    }

    void Controller::GoToItem(uint8_t index) {
        this->current_item_navigate = this->current_menu_navigate->first;

        for (int i = 0; i < index; ++i) {
            this->current_item_navigate = this->current_item_navigate->next;
        }
    }

    void Controller::ExecuteItem() {
        this->GoToItem(this->current_index_selected);
        if (this->current_item_navigate->has_sub_menu) this->GoToMenu((Menu *) this->current_item_navigate->action);
        else (*((void (*)()) this->current_item_navigate->action))();
    }

    void Controller::GoToRoot() {
        this->current_menu_navigate = this->root;
        this->current_index_navigate = 0;
        this->current_index_selected = 0;
    }

    void Controller::GoToParent() {
        this->current_menu_navigate = this->current_menu_control->parent;
        this->current_index_navigate = 0;
        this->current_index_selected = 0;
    }

    void Controller::render() {
        oled->SetNumberOfLines(this->num_lines);
        oled->Clear();

        // Make sure that, if possible, the selected item is not the top or bottom one.
        if (this->current_index_selected - this->current_index_navigate < 1) {
            this->current_index_navigate = min(0, this->current_index_selected - 1);
        } else if (this->current_index_navigate + this->num_lines - this->current_index_selected < 1) {
            this->current_index_navigate = max(this->GetMenuLength(this->current_menu_navigate) - this->num_lines,
                                               this->current_index_selected - this->num_lines + 1);
        }
        this->GoToItem(this->current_index_navigate);

        uint8_t i = 0;
        uint8_t selected_index_relative = this->current_index_selected - this->current_index_navigate;
        do {
            oled->WriteLine(this->current_item_navigate->label, this->current_item_navigate->label_length, i, 2);
            if (i == selected_index_relative)
            i++;
        } while (this->GoToNext());

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

    bool Controller::GoToNext() {
        if (this->current_item_navigate->next == nullptr) return false;
        else {
            this->current_item_navigate = this->current_item_navigate->next;
            return true;
        }
    }

    void Controller::SelectNext() {
        this->current_index_selected = max(this->GetMenuLength(this->current_menu_navigate), this->current_index_selected + 1);
    }

    void Controller::SelectPrevious() {
        this->current_index_selected = min(0, this->current_index_selected - 1);
    }
}
