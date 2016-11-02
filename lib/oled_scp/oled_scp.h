#pragma once
#include "../stream/stream.h"
#include <avr/io.h>
#include "lib/oled/oled.h"
#include "lib/socket/socket.h"
#include "lib/scp/scp.h"

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * An interface to communicate with the oled display
 */
class OLED_SCP : public OLED {

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
};