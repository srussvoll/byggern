#include "../lib/state_machine/state_machine.h"

void (*transitions[][2])(void) = {
/*                                   enter                                    leave                                   */
/* state0                        */ {nullptr                                 ,nullptr                                 },
/* state1                        */ {nullptr                                 ,nullptr                                 },
/* state2                        */ {nullptr                                 ,nullptr                                 },
/* state3                        */ {nullptr                                 ,nullptr                                 },
/* state4                        */ {nullptr                                 ,nullptr                                 },
/* state5                        */ {nullptr                                 ,nullptr                                 },
/* state6                        */ {nullptr                                 ,nullptr                                 },
/* state7                        */ {nullptr                                 ,nullptr                                 },
/* state8                        */ {nullptr                                 ,nullptr                                 },
/* state9                        */ {nullptr                                 ,nullptr                                 },
};

StateMachine fsm((void (**)(void)) transitions);
