#include "menu.h"
#include "../utilities/new.h"
#include <avr/io.h>

namespace Menu {
    Controller::Controller() {
        this->top = new Menu;
        this->current_menu_control = this->top;
    }

    void Controller::FindTop() {
        this->current_menu_control = this->top;
        this->current_item_control = nullptr;
    }

    bool Controller::FindItem(uint8_t index) {
        this->current_item_control = this->current_menu_control->first;

        for (int i = 0; i < index; ++i) {
            if (this->current_item_control->next == nullptr) return false;
            this->current_item_control = this->current_item_control->next;
        }

        return true;
    }

    bool Controller::EnterSubMenu() {
        if (this->current_item_control == nullptr) return false;
        if (!this->current_item_control->has_sub_menu || this->current_item_control->action == nullptr) return false;
        this->current_menu_control = (Menu *) this->current_item_control->action;
        this->current_item_control = nullptr;
        return true;
    }

    bool Controller::EnterSubMenu(uint8_t index) {
        if (!this->FindItem(index)) return false;
        if (!this->current_item_control->has_sub_menu || this->current_item_control->action == nullptr) return false;
        this->current_menu_control = (Menu *) this->current_item_control->action;
        this->current_item_control = nullptr;
        return true;
    }

    bool Controller::FindParent() {
        if (this->current_menu_control->parent == nullptr) return false;
        this->current_menu_control = this->current_menu_control->parent;
        this->current_item_control = nullptr;
        return true;
    }

    void Controller::AddMenu(Item *items, uint8_t length) {

    }

    void Controller::AddMenuItem(Item &item) {
        Item *i = new Item(item);
        if (this->current_item_control == nullptr) {!
            i->next = this->current_menu_control->first;
            this->current_menu_control->first = i;

        } else {
            i->next = this->current_item_control->next;
            this->current_item_control->next = i;
        }
    }

    bool Controller::GoTo(Menu *menu, uint8_t index) {
        return false;
    }

    bool Controller::ExecuteAction(void *action) {
        return false;
    }
}
