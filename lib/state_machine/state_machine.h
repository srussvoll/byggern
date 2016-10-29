#pragma once

#include <stdint.h>

class StateMachine {
private:
    uint8_t current_state;
    void (*(*state_functions))();
    bool running = false;

public:
    StateMachine(void (*(*state_functions))(void), uint8_t initial_state);
    bool Transition(uint8_t state, bool reenter);
    uint8_t GetCurrentState();
    void Start();
    void Stop();
};