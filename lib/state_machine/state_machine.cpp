#include <lib/utilities/printf.h>
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

void StateMachine::Start(){
    this->running = true;
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(this->running){
        uint8_t function_key = 1;
        this->state_functions[3 * this->current_state + function_key]();
    }
    #pragma clang diagnostic pop
}

void StateMachine::Stop(){
    this->running = false;
}