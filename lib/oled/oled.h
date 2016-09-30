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
     */
     void Init(uint8_t width, uint8_t height);

     /**
     *
     */
     void WriteByte(uint8_t page, uint8_t column, uint8_t byte);

     /**
     *
     */
     void WriteByteArray(uint8_t page, uint8_t column, uint8_t *byte_array, uint8_t length);

     /*
     *
     */
     void WriteBitmap(uint8_t x, uint8_t y, uint8_t** bitmap, uint8_t bitmap_height, uint8_t bitmap_width);

     /**
     *
     */
     uint8_t* GetBitmap(uint8_t character, uint8_t **font);

     /**
     *
     */
     void Repaint();

     /**
     *
     */
     void SetNumberOfLines(uint8_t number_of_lines);
     void WriteColumn(uint8_t *pixels, uint8_t num_pixels, uint8_t x, uint8_t y);

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
    * Width of display
    */
    uint16_t display_width = 0;

    /**
    * Height of display
    */
    uint16_t display_height = 0;

    /**
    *
    */
    uint8_t number_of_pages = 0;

    /**
    *
    */

    uint8_t number_of_lines = 0;


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
    uint8_t **matrix;

    /**
    *
    */



    /**
    *
    */
    uint8_t pixels_per_line;


};
