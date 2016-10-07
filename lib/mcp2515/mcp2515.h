#pragma once
#include "../spi/spi.h"
/**
 * A singleton class which implements the communication between the AVR and the MCP2515. Throughout the documentation
 * of this class, we will refer to the datasheet of the chip. This can be found at
 * <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">Microchips site</a>
 */
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
    void Initialize(SPI_N::SPI *spi);

    /**
     * Initiates the loopback mode of the MCP2515. Please consult the
     * <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">MCP2515 datasheet</a>
     */
    void SetLoopback();

private:
    MCP2515(){};
    /**
     * Sends a RTS (Request to Send) signal to the MCP2515. This
     */
    void RequestToSend();
    void WriteToRegister(uint8_t register_address, uint8_t byte);
    uint8_t ReadFromRegister(uint8_t register_address);
    void Reset();
    void BitModify(uint8_t register_address, uint8_t mask, uint8_t data);
    uint8_t ReadStatus();
    SPI_N::SPI *spi_driver;
};