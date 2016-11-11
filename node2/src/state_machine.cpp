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
#include "lib/joystick/joystick.h"
#include "lib/solenoid/solenoid.h"

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

/*-----------------------   INITIALIZE  -------------------------------*/

void InitializeLoop() {
    //TODO: Add initialization to everything for node 2.
    // Initialize the motor
    Motor &motor = Motor::GetInstance();
    motor.Initialize();

    // Initialize the timer
    Timer &timer = Timer::GetInstance();
    timer.Initialize();

    // Initialize the IR Sensor
    IR_Detector::GetInstance().Initialize(0x30);

    // Initialize the joystick
    // TODO: Correct Quantization levels
    Joystick &joy = Joystick::GetInstance();
    Quantization q;
    q.x_max = 0;
    q.x_min = 0;
    q.y_max = 0;
    q.y_min = 0;
    joy.Initialize(q, 0);

    // Initialize the joystick
    Joystick& joystick = Joystick::GetInstance();
    Quantization levels;
    levels.x_min = 0;
    levels.x_max = 255;
    levels.y_min = 0;
    levels.y_max = 255;
    joystick.Initialize(levels, 0.6);

    //Initialize the solenoid
    Solenoid& solenoid = Solenoid::GetInstance();
    solenoid.Initialize();

    fsm->Transition(STATE_IDLE, 0);
}

/*-----------------------   ONGOING  -------------------------------*/

void OngoingEnter() {
    printf("Ongoing enter \n");
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

    Joystick& joystick = Joystick::GetInstance();
    Motor& motor = Motor::GetInstance();

    if(channel->Receive(command, data, length)) {
        if (command == CMD_JOYSTICK_VALUES) {
            x_direction = data[0];
            y_direction = data[1];
            joystick.Update(x_direction, y_direction);
        }
    }

   // printf("X: %d, Y:%d \n", x_direction, y_direction);

    Direction direction = joystick.GetDirection();
    if((direction == West) || (direction == NorthWest) | direction == SouthWest){
        printf("Right \n");
        motor.Drive(joystick.GetPercentageX()*0.5);
        motor.GoRight();
    } else if((direction == East) || (direction == SouthEast) || (direction == NorthEast)){
        printf("Left\n");
        motor.Drive(joystick.GetPercentageX()*0.5);
        motor.GoLeft();
    } else {
        motor.Drive(0);
    }

    if(direction == North){
        if(joystick.DirectionChanged()){
            printf("Solenoid \n");
            Solenoid& solenoid = Solenoid::GetInstance();
            solenoid.Pulse();
        }
    }
    //printf("X: %d, Y:%d \n", x_direction, y_direction);

    // Check fail state
    IR_Detector &ir = IR_Detector::GetInstance();
    if(ir.Sample()) {
        printf("LOST GAME \n");
        fsm->Transition(STATE_IDLE, 0);
        return;
    }
}

void OngoingLeave(){
    printf("Ongoing left\n");
    Motor &motor = Motor::GetInstance();
    motor.Stop();

    Timer &timer = Timer::GetInstance();
    timer.Stop();
    uint16_t time;
    timer.GetFullSecondsPassed(time);
    printf("Time passed = %d \n\r", time);
    uint8_t data[] = {(uint8_t)(time & 0x00FF), (uint8_t)((time & 0xFF00)) >> 8};
    channel->Send(0, CMD_GAME_STOP, data, 2);
}

/*-----------------------   IDLE  -------------------------------*/

void IdleEnter(){
    printf("IDLE STATE ENTERED \n\r");
}

void IdleLoop() {
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

/* States: enter, loops and leaves */
/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, InitializeLoop, nullptr},
/* 1. ONGOING                    */ {OngoingEnter, OngoingLoop, OngoingLeave},
/* 2. IDLE                       */ {IdleEnter, IdleLoop,       nullptr}
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}