#pragma once
#include "../spi/spi.h"
#include "avr/interrupt.h"

#define MCP2515_INT     INT0_vect
/**
 * A singleton class which implements the communication between the AVR and the MCP2515. Throughout the documentation
 * of this class, we will refer to the datasheet of the chip. This can be found at
 * <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">Microchips site</a>
 */
ISR(MCP2515_INT);
struct CAN_MESSAGE{
    uint8_t size;
    uint8_t data;
    uint16_t id;
    CAN_MESSAGE(uint8_t size, uint8_t data, uint16_t id): size(size), data(data), id(id){}
};
class MCP2515{
public:
    /**
     * A Singleton implementation of this class
     *
     */
    static MCP2515& GetInstance(){
        static MCP2515 instance;
        return instance;
    }

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    MCP2515(const MCP2515&) = delete;

    /**
     * Initializes the MCP2515 driver.
     * @param spi The singleton instance of the SPI driver
     */
    void Initialize(SPI_N::SPI *spi, SPI_N::PIN *pin);

    /**
     * Initiates the loopback mode of the MCP2515. Please consult the
     * <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">MCP2515 datasheet</a>
     */
    void SetLoopback();

    void SetNormal();

    /**
     * Sends a RTS (Request to Send) signal to the MCP2515. This is
     */
    void RequestToSend();
    void WriteToRegister(uint8_t register_address, uint8_t byte);
    void ReadFromRegister(uint8_t register_address, uint8_t &byte);
    void Reset();
    void BitModify(uint8_t register_address, uint8_t mask, uint8_t data);
    void ReadStatus(uint8_t &byte);
    void LoadTxFrame(CAN_MESSAGE &message, uint8_t buffer_number);
    void RxStatus(uint8_t &byte);
    void ReadRxFrame(CAN_MESSAGE &message, uint8_t buffer_number);


    friend void MCP2515_INT();

private:
    MCP2515(){};

    // TODO: Implement Interrupts
    SPI_N::SPI *spi_driver;

    SPI_N::PIN *interrupt_pin;

};