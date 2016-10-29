#pragma once

#include <stdint.h>

class StateMachine {
private:
    uint8_t current_state;
    void (*(*state_functions))();
    bool running = false;

public:
    StateMachine(void (*(*state_functions))(void), uint8_t initial_state = 0) : state_functions(state_functions), current_state(initial_state) {
        this->Transition(this->current_state,0);
    }
    bool Transition(uint8_t state, bool reenter);
    uint8_t GetCurrentState();
    void Start();
    void Stop();
};