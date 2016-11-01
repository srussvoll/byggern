#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/uart/uart.h"
#include <util/delay.h>
#include <lib/utilities/printf.h>
#include "../lib/menu/menu.h"
#include "../lib/utilities/fonts.h"
#include "../lib/joystick/joystick.h"
#include "lib/scp/scp.h"
#include "lib/scp/commands.h"
#include <stdio.h>

#define STATE_MENU                  1
#define STATE_GAME                  2
#define STATE_HIGHSCORE             3

StateMachine *fsm;
SCP          *channel;
SOCKET* sockets[] = {
        &SOCKET::GetInstance(0),
        &SOCKET::GetInstance(1)
};

/* States: enter, loops and leaves */
void InitializeLoop(){
    uint8_t command;
    uint8_t length;
    uint8_t data[3];
    if(channel->Receive(command, data, length)) {
        printf("Hmm\n");
        if (command == WRITE_TO_ADDRESS) {
            *((uint8_t*)(((uint16_t)data[0] << 8) + data[1])) = data[2];
        }
    }
}

void MenuLoop() {}
void MenuEnter() {}
void MenuLeave() {}

/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, InitializeLoop, nullptr},
/* 1. Menu                       */ {&MenuEnter, &MenuLoop, &MenuLeave},
/* 2. Play Game                  */ {nullptr,nullptr, nullptr},
/* 3. Highscore Score            */ {nullptr,nullptr, nullptr},
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}