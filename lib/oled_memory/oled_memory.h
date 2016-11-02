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
class OLED_memory : public OLED {

public:
    /**
     * A Singleton implementation of this class
     */
    static OLED_memory& GetInstance(){
        static OLED_memory instance;
        return instance;
    }

    /**
     * Initializes the whole screen.
     * @param width The width of the screen in pixels
     * @param height The height of the screen in pixels
     * @param socket Socket to write through.
    */
    void Init(uint8_t width, uint8_t height);

protected:
    /**
    * Singleton constructor
    */
    OLED_memory() : OLED() {}

    SCP *channel = nullptr;
    uint8_t command;

    virtual void WriteByteToOLED(volatile uint8_t *address, uint8_t data);
};