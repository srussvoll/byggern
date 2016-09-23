#include "../stream/stream.h"
#include <avr/io.h>
/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * An interface to communicate with the oled display
 */
class OLED: Stream {
public:
    /**
     * A Singleton implementation of this class
     *
     */
    static OLED& GetInstance(){
        static OLED instance;
        return instance;
    }

    /**
    * Sets the line pointer. This line pointer is used by functions such as WriteLine() to determine which line to write to.
    * @param line Which line to be set.
    */
     void GoToLine(uint8_t line);

     /**
     * Clears the current line selected
     */
     void ClearLine();

     /**
     * Clears the whole screen
     */
     void Clear();

     /**
     * Initializes the whole screen.
     * @param font The font that is to be used at the OLED display
     */
     void Init(unsigned char font);

     /**
     *
     */



private:
    /**
    * Singleton constructor
    */
    OLED();

    /**
    * The current line used by the driver
    */
    uint8_t current_line = 0;


    /**
    * A pointer to where the OLED_COMMAND address space starts
    */
    volatile uint8_t *oled_command = (volatile uint8_t*)0x3000;

    /**
    * A pointer to where the OLED_DATA address space starts
    */
    volatile uint8_t *oled_data = (volatile uint8_t*)0x3100;

    /**
    *
    */
    uint8_t refresh_rate = 60;

    /**
    *
    */
    uint8_t **matrix;

    /**
    *
    */
    void Repaint();
};
