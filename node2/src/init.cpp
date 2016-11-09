#include "lib/uart/uart.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/spi/spi.h"
#include "lib/socket/socket.h"

inline void InitializeNetworkStack(){
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