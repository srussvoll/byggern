#include <lib/utilities/printf.h>
#include "state_machine.h"

StateMachine::StateMachine(void (*(*state_functions))(void), uint8_t initial_state = 0) : state_functions(state_functions), current_state(initial_state) {
    this->Transition(this->current_state, 0);
}

bool StateMachine::Transition(uint8_t state, bool reenter = false) {
    if (state != this->current_state || (state == this->current_state && reenter)) {
        uint8_t old_state = this->current_state;
        this->current_state = state;

        void (*leave_fn)(void) = this->state_functions[(3 * old_state           + 2)];
        void (*enter_fn)(void) = this->state_functions[(3 * this->current_state + 0)];
        if (leave_fn != nullptr) (*leave_fn)();
        if (enter_fn != nullptr) (*enter_fn)();
        return true;
    } else return false;
}

uint8_t StateMachine::GetCurrentState() {
    return this->current_state;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void StateMachine::Start(){
    this->running = true;
    while(this->running) {
        void (*loop_fn)(void) = this->state_functions[(3 * this->current_state + 1)];
        if (loop_fn != nullptr) (*loop_fn)();
    }
}
#pragma clang diagnostic pop

void StateMachine::Stop(){
    this->running = false;
}