#include <avr/io.h>
#include <util/delay.h>
#include <lib/menu/menu.h>
#include <lib/utilities/fonts.h>
#include <lib/oled_scp/oled_scp.h>
#include "lib/spi/spi.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/socket/socket.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"
#include "lib/scp/commands.h"
#include <lib/dac/dac.h>
#include "lib/socket/socket.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"
#include "lib/adc_internal/adc_internal.h"
#include "state_machine.h"
#include "lib/timer/timer.h"

void init_hardware_drivers() __attribute__((naked)) __attribute((used)) __attribute__((section(".init8")));
void InitializeNetworkStack();
void init_hardware_drivers() {
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    Utilities::EnablePrintf(uart);
    InitializeNetworkStack();
}
void InitializeNetworkStack(){
    // Get instance of all the modules
    SOCKET &high = SOCKET::GetInstance(0x00);
    SOCKET &low = SOCKET::GetInstance(0x01);
    MCP2515 &mcp = MCP2515::GetInstance();
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();

    // Initialize SPI
    SPI_N::PIN ss = SPI_N::PIN(&PORTB, &DDRB, 7);
    SPI_N::PIN ss_a[] = {ss};
    spi.Initialize(ss_a, 1, 0, 0);
    spi.SetDevice(ss);

    // Initialize MCP
    mcp.Initialize(&spi, 0x00);
    //mcp.SetLoopback();
    mcp.SetNormal();

    // Initialize the socket
    high.Initialize(&mcp);
    low.Initialize(&mcp);
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    InitializeStateMachine();
/*    DDRB |= (1<<PB4);
    printf("Start \n");
    Timer &t = Timer::GetInstance();
    t.Initialize();
    t.Start();
    PORTB |= (1<<PORTB4);
    while(t.timer < 5){
    }
    PORTB &= ~(1<<PORTB4);
    printf("DONE\n");
    while(1);*/
}
#pragma clang diagnostic pop