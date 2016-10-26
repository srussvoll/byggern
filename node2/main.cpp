#include "lib/servo/servo.h"
#include <avr/io.h>
#include <util/delay.h>
#include "lib/spi/spi.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/socket/socket.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"

void init_hardware_drivers() __attribute__((naked)) __attribute((used)) __attribute__((section(".init8")));
void init_hardware_drivers() {
    UART& uart = UART::GetInstance();
    uart.Init(9600);
    Utilities::EnablePrintf(uart);
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    // Get instance of all the modules
    SOCKET &s = SOCKET::GetInstance(0x02);
    MCP2515 &mcp = MCP2515::GetInstance();
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();

    // Initialize SPI
    SPI_N::PIN ss = SPI_N::PIN(&PORTB, &DDRB, 7);
    SPI_N::PIN ss_a[] = {ss};
    spi.Initialize(ss_a, 1, 0, 0);
    spi.SetDevice(ss);

    // Initialize MCP
    mcp.Initialize(&spi, 0x02);
    //mcp.SetLoopback();
    mcp.SetNormal();

    // Initialize the socket
    s.Initialize(&mcp, 0x01);
    /*char test_string[] = "En";
    s.Write((uint8_t *)test_string, sizeof(test_string));*/

    _delay_ms(100);

    while(true) {
        uint8_t rec_mes[128];
        printf("Waiting for data...\n");
        while(s.GetAvailableReadBytes() == 0);
        s.Read(rec_mes,128);
        printf("Received: %s\n", (char *) rec_mes);
        _delay_ms(100);
    }
}
/*int main(){
*//*    Servo servo(900, 2100);

    int16_t angle = 0;
    while(true) {
        _delay_ms(1000);
        angle += 20;
        servo.SetAngle(angle);
    }*//*


    // UART
    UART &u = UART::GetInstance();
    u.Init(9600);
    while(1){
        u.WriteByte(0xaa);
    }

*//*    // Get instance of all the modules
    SOCKET &s = SOCKET::GetInstance(1);
    MCP2515 &mcp = MCP2515::GetInstance();
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();

    // Initialize SPI
    SPI_N::PIN ss = SPI_N::PIN(&PORTB, &DDRB, 7);
    SPI_N::PIN ss_a[] = {ss};
    spi.Initialize(ss_a, 1, 0, 0);
    spi.SetDevice(ss);

    // Initialize MCP
    mcp.Initialize(&spi, 0x01);
    mcp.SetLoopback();

    // Initialize the socket
    s.Initialize(&mcp, 1);*//*
}*/
#pragma clang diagnostic pop