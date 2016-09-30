#include "menu.h"
#include "../utilities/new.h"
#include <avr/io.h>

namespace Menu {
    Controller::Controller() {
        this->root = new Menu;
        this->current_menu_control = this->root;
    }

    void Controller::FindRoot() {
        this->current_menu_control = this->root;
        this->current_item_control = nullptr;
    }

    void Controller::FindItem(uint8_t index) {
        this->current_item_control = this->current_menu_control->first;

        for (int i = 0; i < index; ++i) {
            this->current_item_control = this->current_item_control->next;
        }
    }

    void Controller::EnterSubMenu() {
        this->current_menu_control = (Menu *) this->current_item_control->action;
        this->current_item_control = nullptr;
    }

    void Controller::EnterSubMenu(uint8_t index) {
        this->FindItem(index);
        this->current_menu_control = (Menu *) this->current_item_control->action;
        this->current_item_control = nullptr;
    }

    void Controller::FindParent() {
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
            this->FindItem(index - 1);
            i->next = this->current_item_control->next;
            this->current_item_control->next = i;
        }
    }

    void Controller::GoTo(Menu *menu) {
        this->current_menu_navigate = menu;
        this->current_index_navigate = 0;
    }

    void Controller::ExecuteItem(Menu *menu, uint8_t index) {
        Item *item = menu->first;
        for (int i = 0; i < index; ++i) item = item->next;

        if (item->has_sub_menu) this->GoTo((Menu *) item->action);
        else (*((void (*)()) item->action))();
    }

    void Controller::GoToRoot() {
        this->current_menu_navigate = this->root;
        this->current_index_navigate = 0;
    }

    void Controller::GoToParent() {
        this->current_menu_navigate = this->current_menu_control->parent;
        this->current_index_navigate = 0;
    }

    void Controller::render() {

    }
}
