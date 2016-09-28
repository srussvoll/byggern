#include "menu.h"
#include "../utilities/new.h"
#include <avr/io.h>

namespace Menu {
    Controller::Controller() {
        this->current_menu_control = new Menu();
    }

    void Controller::FindTop() {

    }

    bool Controller::Find(uint8_t index) {
        return false;
    }

    bool Controller::FindSubMenu() {
        return false;
    }

    bool Controller::FindSubMenu(uint8_t index) {
        return false;
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
