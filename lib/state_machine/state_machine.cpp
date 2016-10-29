#include "state_machine.h"

bool StateMachine::Transition(uint8_t state, bool reenter = 0) {
    if (state != this->current_state || state == this->current_state && reenter) {
        uint8_t old_state = this->current_state;
        this->current_state = state;

        (*(this->state_functions[(3 * old_state + 1) * sizeof(this->state_functions[0])]))();
        (*(this->state_functions[(3 * this->current_state + 0) * sizeof(this->state_functions[0])]))();
        return true;
    } else return false;
}

uint8_t StateMachine::GetCurrentState() {
    return this->current_state;
}
StateMachine::StateMachine(void (*(*state_functions))(void), uint8_t initial_state = 0) : state_functions(state_functions), current_state(initial_state) { }

void StateMachine::Start(){
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(this->running){
        uint8_t function_key = 1;
        this->state_functions[(3 * this->current_state + function_key) * sizeof(this->state_functions[0])]();
    }
}

void StateMachine::Stop(){
    this->running = false;
}