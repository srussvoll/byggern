#include "init.h"
#include "state_machine.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main(void) {
    InitializeStateMachine(); // This will call an endless loop.
}

#pragma clang diagnostic pop
