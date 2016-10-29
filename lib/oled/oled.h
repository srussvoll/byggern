#pragma once
#include "../stream/stream.h"
#include <avr/io.h>

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * An interface to communicate with the oled display
 */
class OLED : public Stream {

public:
    /**
    * Initializes the whole screen.
     * @param width The width of the screen in pixels
     * @param height The height of the screen in pixels
    */
    void Init(uint8_t width, uint8_t height);

    /**
    * Sets the line pointer. This line pointer is used by functions such as WriteLine() to determine which line to write to.
    * @param line Which line to be set.
    */
    void GoToLine(uint8_t line);

    /**
    * Clears the whole screen
    */
    void Clear();

    /**
    * Clears the current line selected
    */
    void ClearLine();

    /**
    * Writes a byte to the given page and column. This is a helper function mainly used for debugging
    * @param page Which page to be written to
    * @param page Which column to be written to
    * @param byte Which byte to be written
    */
    void WriteByte(uint8_t page, uint8_t column, uint8_t byte);

    /**
    * Writes a bytearray starting at the given page and column. This is a helper function mainly used for debugging
    * @param page Which page to be written to
    * @param page Which column to be written to first
    * @param byte_array Which byte to be written
    * @param length The length of the byte array (number of columns)
    */
    void WriteByteArray(uint8_t page, uint8_t column, uint8_t *byte_array, uint8_t length);

    /**
    * Repaints the OLED
    */
    void Repaint();

    /**
    * Sets the number of lines. Not to be confused with number of pages
    * @param number_of_lines The number of lines.
    */
    void SetNumberOfLines(uint8_t number_of_lines);

    /**
    * Writes a pixel-matrix to the given x and y position on the display
    * @param pixels A double pointer to the matrix
    * @param bitmap_width The width of the bitmap, in pixels
    * @param bitmap_height The height of the bitmap, in pixels
    * @param x The starting position, x direction
    * @param y The starting position, y direction
    * @param is_progmem A bool that indicates where the pixel array is located
    */
    void WriteBitmap(uint8_t **pixels, uint8_t bitmap_width, uint8_t bitmap_height, uint8_t x, uint8_t y, bool is_progmem);

    /**
     *
     * @param font A pointer to the font, usually in progmem
     * @param width The width of the font in pixels
     * @param height The height of the font in pixels
     */
    void SetFont(uint8_t *font, uint8_t width, uint8_t height);

    /**
    * Writes the string to the given line. Erases everything on the line
    * @param string The string
    * @param length Length of the string. No newline of course.
    * @param line The line to be written to
    * @param offset The offset from the left, in characters
    */
    void WriteLine(char* string, uint8_t length, uint8_t line, uint8_t offset);

    /**
     *
     * @param line
     * @return
     */
    uint8_t GetYCoordinateFromLineNumber(uint8_t line);

    /**
    * Returns a pointer to PROGMEM for the bitmap for the given font and character. Emphasis on that it points to PROGMEM.
    * http://www.nongnu.org/avr-libc/user-manual/pgmspace.html
    */
    void GetBitmapForCharacter(char character, uint8_t* &character_bitmap);

protected:
    /**
    * Singleton constructor
    */
    OLED();

    virtual void WriteByteToOLED(volatile uint8_t *address, uint8_t data) {};

    /**
    * The current line used by the driver
    */
    uint8_t current_line = 0;

    /**
    * Width of display
    */
    uint8_t display_width = 0;

    /**
    * Height of display
    */
    uint8_t display_height = 0;

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
    * A matrix for the display
    */
    uint8_t **matrix;

    /**
    * The amount of pixels per line
    */
    uint8_t pixels_per_line;

    /**
    * The font
    */
    uint8_t *font = nullptr;

    /**
    * Width of the font
    */
    uint8_t font_width;

    /**
    * Height of the font
    */
    uint8_t font_height;
};