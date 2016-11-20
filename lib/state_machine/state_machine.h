#pragma once

#include <stdint.h>

/**
 * \brief A C Style state machine implementation.
 *
 * The states are realized by three functions for each state. One that runs when you enter the state,
 * one that loops during the state and one that runs when leaving the state. If you do not want to have
 * a function that runs when you enter or leave the state, a nullptr can be put there in stead.
 *
 * In the state matrix each row is tied to the state, and row 0 - 3 is tied to the state functions.
 *
 * <li> Column 0 is the enter function </li>
 * <li> Column 1 is the loop function </li>
 * <li> Column 2 is the leave function </li>
 *
 * <b>Example</b>
 * \code{.cpp}
 * void (*state_functions[][3])(void) = {
* {nullptr, InitializeLoop, nullptr},
* {OngoingEnter, OngoingLoop, OngoingLeave},
* {IdleEnter, IdleLoop,       nullptr}
* };
 * \endcode
 */
class StateMachine {

private:
    /**
     * A matrix containing the states and their state functions
     */
    void (*(*state_functions))();

    /**
     * A flag indicating if the state machine is running
     */
    bool running = false;


public:

    /**
     * The current state of the state machine
     */
    uint8_t current_state;

    /**
     * Initializer. For the state_functions parameter, it expects a matrix where each row is the state,
     * and column 0 - 2 is the state functions.
     * @param state_functions A matrix containing the states. Please read above
     */
    StateMachine(void (*(*state_functions))(void), uint8_t initial_state = 0) : state_functions(state_functions), current_state(initial_state) {};

    /**
     * Transition to a state.
     * @param state The state to transition to
     * @param reenter If you try to reenter the state you are currently in, this must be true. 
     */
    bool Transition(uint8_t state, bool reenter);
    uint8_t GetCurrentState();
    void Start();
    void Stop();
};