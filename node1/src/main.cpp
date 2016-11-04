#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "../lib/utilities/fonts.h"
#include "init.h"
#include "lib/uart/uart.h"
#include "lib/oled/oled.h"
#include "lib/menu/menu.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/mcp2515/mcp2515_regisers.h"
#include "lib/spi/spi.h"
#include "lib/socket/socket.h"
#include "lib/joystick/joystick.h"
#include "state_machine.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main(void) {
    InitializeStateMachine(); // This will call an endless loop.
/*    volatile uint8_t *address = (uint8_t *)0x2004;
    printf("Write start\n");
    while(1){
        *address = 0x2004;
        //printf("wrote \n");
    }*/
/*
    DDRB |= (1 << DDB0);
    DDRB |= (1 << DDB3);


    SOCKET &socket = SOCKET::GetInstance(0);
    printf("Starting \n");


    while(true) {
        while (socket.GetAvailableReadBytes() < 24);
        char s[40];
        socket.Read((uint8_t *) s, 40);
        printf("%s",s);
    }
*/
}

#pragma clang diagnostic pop
