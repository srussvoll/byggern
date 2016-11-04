#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/scp/scp.h"
#include "../lib/socket/socket.h"
#include "../lib/scp/commands.h"
#include "../lib/motor/motor.h"
#include "../lib/dac/dac.h"
#include <util/delay.h>
#include "../lib/ir_detector/ir_detector.h"
#include "../lib/timer/timer.h"

#define STATE_ONGOING        1
#define STATE_IDLE           2

StateMachine *fsm;
SCP          *channel;
SOCKET* sockets[] = {
        &SOCKET::GetInstance(0),
        &SOCKET::GetInstance(1)
};

uint8_t x_direction = 0;
uint8_t y_direction = 0;

/* States: enter, loops and leaves */
void InitializeLoop() {
    printf("correct node\n");
    //TODO: Add initialization to everything for node 2.

    // Initialize the motor
    Motor &motor = Motor::GetInstance();
    motor.Initialize();

    // Initialize the timer
    Timer &timer = Timer::GetInstance();
    timer.Initialize();

    fsm->Transition(STATE_IDLE, 0);
}

void OngoingInitialize() {
    Motor &motor = Motor::GetInstance();
    motor.Start();
    Timer &timer = Timer::GetInstance();
    timer.Start();
    x_direction = 0;
    y_direction = 0;
}
void OngoingLoop() {
    // Get joystick values
    uint8_t command;
    uint8_t length;
    uint8_t data[2];
    if(channel->Receive(command, data, length)) {
        if (command == CMD_JOYSTICK_VALUES) {
            printf("GOT CMD\n");
            x_direction = data[0];
            y_direction = data[1];
        }
    }
    printf("X: %d, Y:%d \n", x_direction, y_direction);

    // Check fail state
    /*IR_DETECTOR &ir = IR_DETECTOR::GetInstance();
    if(ir.Sample()){
        // Fail state
        fsm->Transition(STATE_IDLE, 0);
        return;
    }*/
}

void OngoingLeave(){
    Motor &motor = Motor::GetInstance();
    motor.Stop();

    Timer &timer = Timer::GetInstance();
    timer.Stop();
    uint16_t time;
    timer.GetFullSecondsPassed(time);
    uint8_t data[] = {(uint8_t)((time & 0xFF00)) >> 8, (uint8_t)(time & 0x00FF)};
    channel->Send(0, CMD_GAME_STOP, data, 2);
}

void IdleLoop() {
    printf("Idling\n");
    uint8_t command;
    uint8_t length;
    uint8_t data[1];
    if(channel->Receive(command, data, length)) {
        if (command == CMD_GAME_START) {
            fsm->Transition(STATE_ONGOING, 0);
            return;
        }
    }
}

/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, InitializeLoop, nullptr},
/* 1. ONGOING                    */ {OngoingInitialize, OngoingLoop, OngoingLeave},
/* 2. IDLE                       */ {nullptr, IdleLoop,       nullptr}
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}