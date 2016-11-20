#pragma once

#include <avr/io.h>

#include "../stream/stream.h"
#include "lib/oled/oled.h"
#include "lib/socket/socket.h"
#include "lib/scp/scp.h"

/**
 * \brief A class to communicate with the OLED display using the external memory interface.
 */
class OLED_memory : public OLED {

protected:
    /**
     * Singleton constructor.
     */
    OLED_memory() : OLED() {}

    /**
     * See the OLED.WriteByteToOLED() for more information.
     */
    virtual void WriteByteToOLED(volatile uint8_t *address, uint8_t data);


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
     * @param width The width of the screen in pixels.
     * @param height The height of the screen in pixels.
     */
    void Init(uint8_t width, uint8_t height);
};