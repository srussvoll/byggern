#include "state_machine.h"
#include "../lib/state_machine/state_machine.h"
#include "../lib/scp/scp.h"
#include "../lib/socket/socket.h"

#define ONGOING        0;
#define IDLE           1;

StateMachine *fsm;
SCP          *channel;
SOCKET* sockets[] = {
        &SOCKET::GetInstance(0),
        &SOCKET::GetInstance(1)
};

/* States: enter, loops and leaves */
void InitializeLoop(){
    // Initialize joystick
}


/* State functions table */
void (*state_functions[][3])(void) = {
/* 0. Initialize                 */ {nullptr, nullptr, nullptr},
/* 1. Menu                       */ {nullptr, nullptr, nullptr},
/* 2. Play Game                  */ {nullptr, nullptr, nullptr},
/* 3. Highscore Score            */ {nullptr,nullptr, nullptr},
};

/* Initialize and start the state machine */
void InitializeStateMachine(){
    channel = new SCP(sockets, 2);
    fsm = new StateMachine((void (**)(void)) state_functions, 0);
    fsm->Start();
}