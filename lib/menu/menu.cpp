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
    }

    bool Controller::Find(uint8_t index) {
        this->current_item_control = this->current_menu_control->first;

        for (int i = 0; i < index; ++i) {
            if (this->current_item_control->next == nullptr) return false;
            this->current_item_control = this->current_item_control->next;
        }

        return true;
    }

    bool Controller::FindSubMenu() {
        if (this->current_item_control == nullptr) return false;
        if (!this->current_item_control->has_sub_menu || this->current_item_control->action == nullptr) return false;
        this->current_menu_control = (Menu *) this->current_item_control->action;
        return true;
    }

    bool Controller::FindSubMenu(uint8_t index) {
        if (!this->Find(index)) return false;
        if (!this->current_item_control->has_sub_menu || this->current_item_control->action == nullptr) return false;
        this->current_menu_control = (Menu *) this->current_item_control->action;
        return true;
    }

    bool Controller::FindParent() {
        return false;
    }

    void Controller::AddMenu(Item *items, uint8_t length) {

    }

    void Controller::AddMenuItem(Item item) {

    }

    bool Controller::GoTo(Menu *menu, uint8_t index) {
        return false;
    }

    bool Controller::ExecuteAction(void *action) {
        return false;
    }
}
