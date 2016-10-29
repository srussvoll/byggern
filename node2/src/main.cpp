#include "lib/servo/servo.h"
#include <avr/io.h>
#include <util/delay.h>
#include <lib/menu/menu.h>
#include <lib/utilities/fonts.h>
#include "lib/spi/spi.h"
#include "lib/mcp2515/mcp2515.h"
#include "lib/socket/socket.h"
#include "lib/uart/uart.h"
#include "lib/utilities/printf.h"

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
    SOCKET &s = SOCKET::GetInstance(0x01);
    MCP2515 &mcp = MCP2515::GetInstance();
    SPI_N::SPI &spi = SPI_N::SPI::GetInstance();

    // Initialize SPI
    SPI_N::PIN ss = SPI_N::PIN(&PORTB, &DDRB, 7);
    SPI_N::PIN ss_a[] = {ss};
    spi.Initialize(ss_a, 1, 0, 0);
    spi.SetDevice(ss);

    // Initialize MCP
    mcp.Initialize(&spi, 0x01);
    //mcp.SetLoopback();
    mcp.SetNormal();

    // Initialize the socket
    s.Initialize(&mcp, 0x01);
}

void test_oled() {
    SOCKET &sock = SOCKET::GetInstance(0x01);

    printf("\nTesting menu.\n");

    OLED &my_oled = OLED_CAN::GetInstance();
    my_oled.Init(128,64);
    my_oled.SetFont(Fonts::f8x8, 8, 8);

    Menu::Controller controller(my_oled, 5);

    printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);

    char item0[] = "Menylinje 0";
    char item1[] = "Menylinje 1";
    char item2[] = "Menylinje 2";
    char item3[] = "Menylinje 3";
    char item4[] = "Menylinje 4";
    char item5[] = "Menylinje 5";
    char item6[] = "Menylinje 6";
    char item7[] = "Menylinje 7";
    char item8[] = "Submeny 1";
    char item9[] = "Submeny 2";

    //printf("SP: %d %% used\n", ((0x4FF - SP) * 100) / 0x400);

    Menu::Item *i0 = new Menu::Item(item0, sizeof(item0) - 1);
    Menu::Item *i1 = new Menu::Item(item1, sizeof(item1) - 1);
    Menu::Item *i2 = new Menu::Item(item2, sizeof(item2) - 1);
    Menu::Item *i3 = new Menu::Item(item3, sizeof(item3) - 1);
    Menu::Item *i4 = new Menu::Item(item4, sizeof(item4) - 1);
    Menu::Item *i5 = new Menu::Item(item5, sizeof(item5) - 1);
    Menu::Item *i6 = new Menu::Item(item6, sizeof(item6) - 1);
    Menu::Item *i7 = new Menu::Item(item7, sizeof(item7) - 1);

    Menu::Item *i8 = new Menu::Item(item8, sizeof(item8) - 1);
    Menu::Item *i9 = new Menu::Item(item9, sizeof(item9) - 1);




    Menu::Item* main_items[] = {i0, i1, i2, i3, i4, i5, i6, i7};
    Menu::Item* sub_main_items[] = {i8,i9};

    printf("Sizeof menuitem: %d\n", sizeof(main_items));

    controller.AddMenuItems(main_items, (sizeof(main_items)) / sizeof(main_items[0]));

    controller.ControlGoToItem(4);
    controller.AddMenu(sub_main_items, (sizeof(sub_main_items)) / sizeof(sub_main_items[0]));

    controller.Render();

    for (uint8_t i = 0; i < 4; ++i) {
        _delay_ms(200);
        controller.SelectNext();
    }

    controller.ExecuteItem();
    controller.Render();

    for (uint8_t i = 0; i < 1; ++i) {
        _delay_ms(200);
        controller.SelectNext();
    }

    controller.GoToParent();
    for (uint8_t i = 0; i < 4; ++i) {
        _delay_ms(200);
        controller.SelectNext();
    }

}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    test_oled();
}
#pragma clang diagnostic pop