#pragma once
#include "../spi/spi.h"
#include "avr/interrupt.h"
#include "../can/can.h"

#define MCP2515_INT     INT0_vect
/**
 * A singleton class which implements the communication between the AVR and the MCP2515. Throughout the documentation
 * of this class, we will refer to the datasheet of the chip. This can be found at
 * <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">Microchips site</a>
 */
ISR(MCP2515_INT);

class MCP2515: public CAN{
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

    friend void MCP2515_INT();

    // CAN specific functions
    void SendMessage(CAN_MESSAGE &message);

private:
    /**
    * Initializes the MCP2515 driver.
    * @param spi The singleton instance of the SPI driver
    */
    void Initialize(SPI_N::SPI *spi, SPI_N::PIN *pin, uint16_t identifier);

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
    void LoadTxFrame(CAN_MESSAGE &message);
    void RxStatus(uint8_t &byte);
    void ReadRxFrame(CAN_MESSAGE &message);
    MCP2515(){};

    SPI_N::SPI *spi_driver;

    SPI_N::PIN *interrupt_pin;

    bool clear_to_send = true;


};