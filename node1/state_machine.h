#pragma once

#include <stdint.h>

namespace StateMachine {
    extern uint8_t current_state;
    extern void (*transitions[][2])(void);
    bool transition(uint8_t state, bool reenter);
}