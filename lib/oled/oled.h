#pragma once

#include <avr/io.h>

#include "../stream/stream.h"

/**
 * \brief An interface to communicate with the OLED display.
 */
class OLED : public Stream {

protected:

    /**
    * Singleton constructor.
    */
    OLED();

    /**
     * Writes a single byte to the OLED.
     * This can be implemented using the external memory interface
     * or using whatever other technology like for instance the SCP.
     * @param address The address to write to.
     * @param data The data to write.
     */
    virtual void WriteByteToOLED(volatile uint8_t *address, uint8_t data) {};

    /**
     * The current line to write to.
     */
    uint8_t current_line = 0;

    /**
     * Width of the display in pixels.
     */
    uint8_t display_width = 0;

    /**
     * Height of display in pixels.
     */
    uint8_t display_height = 0;

    /**
     * Number of pages
     */
    uint8_t number_of_pages = 0;

    /**
     * Number of lines. Not to be confused with number of pages.
     */
    uint8_t number_of_lines = 0;

    /**
     * A pointer to where the OLED_COMMAND address space starts.
     */
    volatile uint8_t *oled_command = (volatile uint8_t*)0x8000;

    /**
     * A pointer to where the OLED_DATA address space starts.
     */
    volatile uint8_t *oled_data = (volatile uint8_t*)0x8100;

    /**
     * The display buffer. There is also a buffer on the OLED controller,
     * such that this implements a dual buffer.
     */
    uint8_t **matrix;

    /**
     * The number of pixels per line.
     */
    uint8_t pixels_per_line;

    /**
     * The font to use for writing.
     */
    uint8_t *font = nullptr;

    /**
     * Width of the font in pixels.
     */
    uint8_t font_width;

    /**
     * Height of the font in pixels.
     */
    uint8_t font_height;

    /**
     * Fetches a pointer to PROGMEM for the bitmap for the given font and character.
     * Emphasis on that it points to PROGMEM.
     */
    void GetBitmapForCharacter(char character, uint8_t* &character_bitmap);


public:

    /**
     * Initializes the whole screen.
     * @param width The width of the screen in pixels.
     * @param height The height of the screen in pixels.
     */
    void Init(uint8_t width, uint8_t height);

    /**
     * Sets the line pointer.
     * @param line Which line to go to.
     */
    void GoToLine(uint8_t line);

    /**
     * Clears the whole screen.
     */
    void Clear();

    /**
     * Clears the current line.
     */
    void ClearLine();

    /**
     * Writes a byte to the given page and column. This is a helper function mainly used for debugging.
     * @param page Which page to be written to.
     * @param page Which column to be written to.
     * @param byte The byte to be written.
     */
    void WriteByte(uint8_t page, uint8_t column, uint8_t byte);

    /**
     * Writes a byte array starting at the given page and column. This is a helper function mainly used for debugging.
     * @param page Which page to be written to.
     * @param page Which column to be written to first.
     * @param byte_array Bytes to be written.
     * @param length The length of the byte array (number of columns).
     */
    void WriteByteArray(uint8_t page, uint8_t column, uint8_t *byte_array, uint8_t length);

    /**
     * Repaints the OLED
     */
    void Repaint();

    /**
     * Sets the number of lines. Not to be confused with number of pages.
     * @param number_of_lines The number of lines.
     */
    void SetNumberOfLines(uint8_t number_of_lines);

    /**
     * Writes a pixel matrix to the given x and y position on the display.
     *
     * @param pixels A double pointer to the matrix.
     * @param bitmap_width The width of the bitmap in pixels.
     * @param bitmap_height The height of the bitmap in pixels.
     * @param x The starting position, x direction.
     * @param y The starting position, y direction.
     * @param is_progmem A bool that indicates where the pixel array is located.
     */
    void WriteBitmap(uint8_t **pixels, uint8_t bitmap_width, uint8_t bitmap_height, uint8_t x, uint8_t y, bool is_progmem);

    /**
     * Sets the font to be used when writing to the screen.
     *
     * @param font A pointer to the font. Put this on the PROGMEM only if possible to save RAM space.
     * @param width The width of the font in pixels.
     * @param height The height of the font in pixels.
     */
    void SetFont(uint8_t *font, uint8_t width, uint8_t height);

    /**
     * Writes a string to the given line.
     *
     * @param string The string to be written.
     * @param length Length of the string.
     * @param line The line to write to.
     * @param offset The offset from the left, in characters.
     */
    void WriteLine(char* string, uint8_t length, uint8_t line, uint8_t offset);

    /**
     * Returns the y coordinate of the line.
     * @param line The line to find the y coordinate of.
     * @return The y coordinate of the line.
     */
    uint8_t GetYCoordinateFromLineNumber(uint8_t line);

    /**
     * Returns how wide a line is in characters.
     * @return How wide a line is in characters.
     */
    uint8_t GetMaxLineCharacters();
};