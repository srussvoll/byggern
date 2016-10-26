#include "state_machine.h"

bool StateMachine::Transition(uint8_t state, bool reenter = 0) {
    if (state != this->current_state || state == this->current_state && reenter) {
        uint8_t old_state = this->current_state;
        this->current_state = state;

        (*(this->transitions[(2 * old_state           + 1) * sizeof(this->transitions[0])]))();
        (*(this->transitions[(2 * this->current_state + 0) * sizeof(this->transitions[0])]))();
        return true;
    } else return false;
}

uint8_t StateMachine::GetCurrentState() {
    return this->current_state;
}
