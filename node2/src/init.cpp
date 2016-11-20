#include "init.h"
#include "lib/uart/uart.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/spi/spi.h"
#include "lib/socket/socket.h"
#include <lib/utilities/printf.h>

inline void InitializeNetworkStack(){
    // Get instance of all the modules
    Socket &high = Socket::GetInstance(0x00);
    Socket &low = Socket::GetInstance(0x01);
    MCP2515 &mcp = MCP2515::GetInstance();
    SPI::SPI &spi = SPI::SPI::GetInstance();

    // Initialize SPI
    SPI::PIN ss = SPI::PIN(&PORTB, &DDRB, 7);
    SPI::PIN nordic_pin = SPI::PIN(&PORTG, &DDRG, 1);
    SPI::PIN ss_a[] = {ss, nordic_pin};
    spi.Initialize(ss_a, 2, 0, 0);
    spi.SetDevice(ss);

    // Initialize MCP
    mcp.Initialize(&spi, 0x00);
    mcp.SetNormal();

    // Initialize the socket
    high.Initialize(&mcp);
    low.Initialize(&mcp);
}

inline void InitializeUART() {
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    Utilities::EnablePrintf(uart);
}


void init_hardware_drivers() {
    InitializeUART();
    InitializeNetworkStack();
}