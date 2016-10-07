#pragma once
#include "../spi/spi.h"
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

    void initialize(SPI_N::SPI *spi, SPI_N::PIN ss_pin);

    void SetLoopback();

private:
    MCP2515(){};
    void RequestToSend();
    void WriteToRegister(uint8_t register_address, uint8_t byte);
    uint8_t ReadFromRegister(uint8_t register_address);
    void Reset();
    void BitModify(uint8_t register_address, uint8_t mask, uint8_t data);
    uint8_t ReadStatus();
    SPI_N::PIN ss_pin;
    SPI_N::SPI *spi_driver;
};