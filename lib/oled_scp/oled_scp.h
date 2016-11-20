#pragma once

#include <avr/io.h>

#include "../stream/stream.h"
#include "lib/oled/oled.h"
#include "lib/socket/socket.h"
#include "lib/scp/scp.h"

/**
 * \brief A class to communicate with the OLED display through the SCP.
 */
class OLED_SCP : public OLED {

protected:
    /**
    * Singleton constructor
    */
    OLED_SCP() : OLED() {}

    SCP *channel = nullptr;
    uint8_t command;

    uint8_t *bytes_to_send;
    uint8_t bytes_to_send_index = 0;

    virtual void WriteByteToOLED(volatile uint8_t *address, uint8_t data);


public:

    /**
     * A Singleton implementation of this class
     */
    static OLED_SCP& GetInstance(){
        static OLED_SCP instance;
        return instance;
    }

    /**
     * Initializes the whole screen.
     * @param width The width of the screen in pixels
     * @param height The height of the screen in pixels
     * @param socket Socket to write through.
    */
    void Init(uint8_t width, uint8_t height, SCP &channel, uint8_t command);
};