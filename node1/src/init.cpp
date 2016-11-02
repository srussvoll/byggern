#ifdef __AVR_ATmega162__

#include "init.h"

#include <avr/io.h>
#include "lib/utilities/memory.h"
#include "lib/utilities/printf.h"
#include "lib/uart/uart.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/spi/spi.h"
#include "lib/socket/socket.h"

void InitializeNetworkStack(){
    // Get instance of all the modules
    SOCKET &s = SOCKET::GetInstance(0);
    MCP2515 &mcp = MCP2515::GetInstance();
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();

    // Initialize SPI
    SPI_N::PIN ss = SPI_N::PIN(&PORTD, &DDRD, 5);
    SPI_N::PIN ss_a[] = {ss};
    spi.Initialize(ss_a, 1, 0, 0);
    spi.SetDevice(ss);

    // Initialize MCP
    mcp.Initialize(&spi, 0);
    //mcp.SetLoopback();
    mcp.SetNormal();

    // Initialize the socket
    s.Initialize(&mcp);
    printf("Initialized \n");
}

void init_memory() {
    Utilities::InitializeMemory();
}
// FIXME: REMOVE THIS!
extern "C" void __cxa_pure_virtual() {  }
void init_hardware_drivers() {
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    Utilities::EnablePrintf(uart);
    InitializeNetworkStack();
}

#elif __AVR_ATmega2560__

// Add the init for  node2

#endif
