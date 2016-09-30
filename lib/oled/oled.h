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
    // void WriteBitmap(uint8_t x, uint8_t y, uint8_t** bitmap, uint8_t bitmap_height, uint8_t bitmap_width, uint8_t is_progmem);

     /**
     * Returns a pointer to PROGMEM for the bitmap for the given font and character. Emphasis on that it points to PROGMEM.
     * http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
     */
     uint8_t** GetBitmapForCharacter(uint8_t character, uint8_t ***font);

     /**
     * Repaints the OLED
     */
     void Repaint();

     /**
     * Sets the number of lines. Not to be confused with number of pages
     * @param number_of_lines The number of lines.
     */
     void SetNumberOfLines(uint8_t number_of_lines);


     void WriteBitmap(uint8_t **pixels, uint8_t bitmap_width, uint8_t bitmap_height, uint8_t x, uint8_t y, uint8_t is_progmem);

     /**
     * Writes the given string to the line set by current line. Uses the supplied font. Assumes that the given font is not higher than the line height.
     * @param string The string to be written
     * @param length_of_string The length of the string
     * @param font The font as an double array. ASCII ordering with an offset of 32
     * @param font_height The height in pixels of the font.
     * @param font_width The width in pixels of the font
     */
     void WriteLine(uint8_t *string, uint8_t length_of_string, uint8_t ***font, uint8_t font_height, uint8_t font_width, uint8_t offset = 0);

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
    * Number of pages
    */
    uint8_t number_of_pages = 0;

    /**
    * Number of lines. Not to be confused with number of pages
    */
    uint8_t number_of_lines = 0;

    /**
    * A pointer to where the OLED_COMMAND address space starts
    */
    volatile uint8_t *oled_command = (volatile uint8_t*)0x8000;

    /**
    * A pointer to where the OLED_DATA address space starts
    */
    volatile uint8_t *oled_data = (volatile uint8_t*)0x8100;

    /**
    * A buffer where the entierty of the OLED data is stored
    */
    uint8_t **matrix;

    /**
    * How many pixels there are per line
    */
    uint8_t pixels_per_line;


};
