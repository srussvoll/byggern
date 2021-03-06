#pragma once

#include "../spi/spi.h"
#include "avr/interrupt.h"
#include "../can/can.h"

#ifdef __AVR_ATmega162__

#define MCP2515_INT INT0_vect

#else

#define MCP2515_INT INT4_vect

#endif

ISR(MCP2515_INT);

/**
 * \brief A singleton class which implements the communication between the AVR and the MCP2515.
 *
 * Throughout the documentation
 * of this class, we will refer to the datasheet of the chip. This can be found at
 * <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">Microchips site</a>
 *
 * The SPI driver you are using must have meet the following criteria
 * <li>Size of output buffer > 14 bytes </li>
 * <li>Size of input buffer > 11 bytes </li>
 *
 * <b>Please note:</b> This implementation only uses one recieve and transmit buffer. TX0 and RX0
 */
class MCP2515: public CAN{

private:

    /**
     * Sends a RTS (Request to Send) signal to the MCP2515.
     */
    void RequestToSend();

    /**
     * Writes to the given MCP2515 register
     * @param register_address The address to the MCP2515 register
     * @param byte The byte to be written to the given register
     */
    void WriteToRegister(uint8_t register_address, uint8_t byte);

    /**
     * Reads from the given MCP2515 register
     * @param register_address The address to the MCP2515 register
     * @param byte The byte to be written into
     */
    void ReadFromRegister(uint8_t register_address, uint8_t &byte);

    /**
     * Sends a reset command to the MCP2515 thorugh the SPI interace (hardware reset not connected).
     * Will put the MCP2515 in config mode.
     */
    void Reset();

    /**
     * Modifies the given MCP2515 register with a bitmask. Please see <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">MCP2515 datasheet</a>
     * page 66
     * @param register_address The address to the MCP2515 register
     * @param mask Defines which bits to be modified
     * @param data The byte to be written
     */
    void BitModify(uint8_t register_address, uint8_t mask, uint8_t data);

    /**
     * Gets the status byte from the MCP2515. Please see <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">MCP2515 datasheet</a>
     * page 65, <i>READ STATUS instruction</i>
     * @param byte The byte to be written into
     */
    void ReadStatus(uint8_t &byte);

    /**
     * Loads the transmit frame, i.e. sends the CAN data to the MCP2515
     * @param message The CAN_MESSAGE to be loaded into the TX frame
     */
    void LoadTxFrame(CanMessage &message);

    /**
     * Gets the RxStatus byte from the MCP2515. Please see <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">MCP2515 datasheet</a>
     * page 66, <i>RX STATUS instruction</i>
     * @param byte The byte to be written into
     */
    void RxStatus(uint8_t &byte);

    /**
     * Reads the RxFrame, i.e. retrieves the recieved data
     * @param message The message to be written into
     */
    void ReadRxFrame(CanMessage &message);

    /**
     * Initializer not used due to singleton
     */
    MCP2515(){};

    /**
     * The SPI Driver the MCP2515 uses.
     */
    SPI::SPI *spi_driver;

    /**
     * Flag indicating if we can load the TX frame.
     */
    volatile bool clear_to_send = true;

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
     * The MCP2515 interrupt handler
     */
    friend void MCP2515_INT();

    /**
     * Sends a CAN message
     * @param message The CAN message to be sent
     */
    void SendMessage(CanMessage &message);

    /**
     * Initiates the loopback mode of the MCP2515. Please consult the
     * <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">MCP2515 datasheet</a>
     */
    void SetLoopback();

    /**
     * Initiates the normal mode of the MCP2515. Please consult the
     * <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/21801G.pdf">MCP2515 datasheet</a>
     */
    void SetNormal();

    /**
    * Initializes the MCP2515 driver.
    * @param spi The singleton instance of the SPI driver
    * @param identifier The CAN identifier for this node
    */
    void Initialize(SPI::SPI *spi, uint16_t identifier);
};