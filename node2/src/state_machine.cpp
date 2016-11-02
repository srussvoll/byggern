#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/scp/scp.h"
#include "../lib/socket/socket.h"
#include "../lib/scp/commands.h"
#include "../lib/motor/motor.h"
#include "../lib/dac/dac.h"
#include <util/delay.h>
#include "../lib/joystick/direction.h"

#define STATE_ONGOING        1
#define STATE_IDLE           2

StateMachine *fsm;
SCP          *channel;
SOCKET* sockets[] = {
        &SOCKET::GetInstance(0),
        &SOCKET::GetInstance(1)
};
uint8_t current_direction;

/* States: enter, loops and leaves */
void InitializeLoop() {
    printf("correct node\n");
    //TODO: Add initialization to everything for node 2.

    Motor &motor = Motor::GetInstance();
    motor.Initialize();

    fsm->Transition(STATE_IDLE, 0);
}

void OngoingInitialize() {
    current_direction = LEFT;
    Motor &motor = Motor::GetInstance();
    motor.Start();
}
void OngoingLoop() {
    uint8_t command;
    uint8_t length;
    uint8_t data[2];
    uint8_t x_direction = UNDEFINED;
    uint8_t y_direction = UNDEFINED;
    Motor &motor = Motor::GetInstance();

    if(channel->Receive(command, data, length)) {
        if (command == CMD_JOYSTICK_VALUES) {
            printf("GOT CMD\n");
            x_direction = data[0];
            y_direction = data[1];
        }
    }

    if(x_direction != current_direction){
        motor.ChangeDirection();
        if(current_direction == RIGHT){
            current_direction = LEFT;
        }else{
            current_direction = RIGHT;
        }
    }
}

void IdleLoop() {
    printf("Idling\n");
    uint8_t command;
    uint8_t length;
    uint8_t data[1];
    if(channel->Receive(command, data, length)) {
        if (command == CMD_GAME_START) {
            fsm->Transition(STATE_ONGOING, 0);
        }
    }
}

/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, &InitializeLoop, nullptr},
/* 1. ONGOING                    */ {&OngoingInitialize, &OngoingLoop,    nullptr},
/* 2. IDLE                       */ {nullptr, &IdleLoop,       nullptr}
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}