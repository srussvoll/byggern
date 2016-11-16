#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "../lib/utilities/fonts.h"
#include "init.h"
#include "lib/uart/uart.h"
#include "lib/oled_memory/oled_memory.h"
#include "lib/menu/menu.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/mcp2515/mcp2515_regisers.h"
#include "lib/spi/spi.h"
#include "lib/socket/socket.h"
#include "lib/joystick/joystick.h"
#include "state_machine.h"
#include "lib/highscore/highscore.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main(void) {
    _delay_ms(100);
    InitializeStateMachine(); // This will call an endless loop.
}

#pragma clang diagnostic pop
