#pragma once

#include <string.h>
#include <avr/io.h>
/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø, Sondre Russvoll
 * @version 1.0
 *
 * An interface for handling streams.
 */

/**
 * This class is an interface for implementing a duplex FIFO stream.
 *
 * This class implements a duplex FIFO stream using two circular buffers.
 * For instance both an RS232 driver and a CAN driver would need to implement a stream.
 * Inheriting from this interface makes the comunication interface for RS232, CAN and other stream based
 * classes/drivers the same, meaning the end user does not need to know whether the data being sent is being sent
 * over a UART or a CAN bus.
 *
 * As an example you can set up a CAN bus using a CAN driver inheriting from this interface. Then it can be
 * passed to a function expecting to send data to an RS232 class also inheriting from this interface, and it will
 * just work. This interface is in other words written to make it easier to write consistent and driver independent code.
 */
class Stream {
protected:

    /**
     * Buffer that stores the input stream data.
     */
    uint8_t *input_buffer = nullptr;

    /**
     * Buffer that stores the output stream data.
     */
    uint8_t *output_buffer = nullptr;

    /**
     * Indicates the first valid data byte in the buffer.
     */
    uint16_t input_buffer_start_index = 0;

    /**
     * Indicates the first valid data byte in the buffer.
     */
    uint16_t output_buffer_start_index = 0;

    /**
     * Indicates the last valid data byte in the buffer.
    */
    uint16_t input_buffer_stop_index;

    /**
     * Indicates the last valid data byte in the buffer.
     */
    uint16_t output_buffer_stop_index;

    /**
     * The size of the input buffer.
     */
    uint16_t input_buffer_size;

    /**
     * The size of the output buffer.
     */
    uint16_t output_buffer_size;

    /**
     * Flag indicating whether the buffer is empty or full.
     */

    bool input_buffer_empty = true;

    /**
     * Flag indicating whether the buffer is empty or full.
     */

    bool output_buffer_empty = true;

    /**
     * Flag indicating whether the input buffer has overflowed or not.
     * This is reset when read by CheckInputOverflowFlag().
     */
    bool input_buffer_overflowed = false;

    /**
     * Flag indicating whether the output buffer has overflowed or not.
     * This is reset when read by CheckOutputOverflowFlag().
     */
    bool output_buffer_overflowed = false;


    /**
     * Reads a string from the given buffer.
     * @param buffer Buffer to read from.
     * @param start_index The first valid byte of the buffer.
     * @param stop_index The last valid byte of the buffer.
     * @param buffer_size The size of the buffer.
     * @param empty Flag indicating whether the buffer is empty or completely full.
     * @param string The string to read into.
     * @param string_size The size of the string.
     */
    virtual void ReadFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty, uint8_t *string, uint16_t &string_size);
    /**
     * Writes a string to the given buffer.
     * @param buffer Buffer to write to.
     * @param start_index The first valid byte of the buffer.
     * @param stop_index The last valid byte of the buffer.
     * @param buffer_size The size of the buffer.
     * @param empty Flag indicating whether the buffer is empty or completely full.
     * @param overflow_flag A flag indicating whether or not the buffer has overflowed.
     * @param string The string to read from.
     * @param string_size The size of the string.
     */
    virtual void WriteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty, bool &overflow_flag, uint8_t *string, uint16_t &string_size);

    /**
     * Reads a byte from the given buffer.
     * @param buffer The buffer to read from.
     * @param start_index The first valid bit of the buffer.
     * @param stop_index The last valid bit of the buffer.
     * @param buffer_size The size of the buffer.
     * @param empty Flag indicating whether the buffer is empty or completely full.
     * @return The byte that was read.
     */
    virtual uint8_t ReadByteFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty);

    /**
     * Writes a byte to the buffer.
     * @param buffer The buffer to write to.
     * @param start_index The first valid bit of the buffer.
     * @param stop_index The last valid bit of the buffer.
     * @param buffer_size The size of the buffer.
     * @param empty Flag indicating whether the buffer is empty or completely full.
     * @param overflow_flag A flag indicating whether or not the buffer has overflowed.
     * @param byte The byte to be written.
     */
    virtual void WriteByteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty, bool &overflow_flag, uint8_t &byte);

    /**
     * Writes a byte to the input stream.
     * @param byte The byte to be written.
     */
    virtual void WriteByteToInputStream(uint8_t byte);

    /**
     * Reads a byte from the output stream.
     * @return Returns the byte.
     */
    virtual void ReadByteFromOutputStream(uint8_t *byte);

    /**
     * Writes the specified data to the output stream.
     * @param string Input data.
     * @param size Size of the input data.
     */
    virtual void WriteToInputStream(uint8_t *string, uint16_t size);

    /**
     * Reads data from the input stream into the specified string.
     * @param string String to store the read data in.
     * @param size Size of the string.
     */
    virtual void ReadFromOutputStream(uint8_t *string, uint16_t size);

    /**
     * Calculates the length of the valid part of the buffer.
     * @param start_index The start index of the buffer.
     * @param stop_index The stop index of the buffer.
     * @return Length of valid data.
     */
    // TODO: Try this with inline
    virtual uint16_t CalculateLength(uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty);

    /**
    * Calculates the length of the readable part of the buffer.
    * @return Length of valid data.
    */
    virtual uint16_t GetInputBufferLength();

    /**
    * Calculates the length of the readable part of the buffer.
    * @return Length of valid data
    */
    virtual uint16_t GetOutputBufferLength();

public:
    /**
     * Used to initialize the stream sizes and other data members.
     * @param input_stream_size The maximum size of the input stream.
     * @param output_stream_size The maximum size of the output stream.
     */
    Stream(uint16_t input_stream_size, uint16_t output_stream_size);

    /**
     * Writes the specified data to the output stream.
     * @param string Input data.
     * @param size Size of the input data string.
     */
    virtual void Write(uint8_t *string, uint16_t size);

    /**
     * Reads data from the input stream into the specified string.
     * @param string String to store read data in.
     * @param size Size of the string.
     */
    virtual void Read(uint8_t *string, uint16_t size);

    /**
     * Reads one byte from the input stream.
     * @return Returns the read byte.
     */
    virtual uint8_t ReadByte();

    /**
     * Writes one byte to the output stream.
     * @param byte The byte to be written.
     */
    virtual void WriteByte(uint8_t byte);

    /**
     * Returns number of bytes not being used in the output buffer (size - length)
     * @return Returns the length of the free space in the output stream.
     */
    virtual uint8_t GetAvailableWriteBytes();

    /**
     * Returns the number of bytes available for reading.
     * @return Returns the length of valid data in the input stream.
     */
    virtual uint8_t GetAvailableReadBytes();

    /**
     * Checks whether or not the input overflow flag has been set. This also clears the flag.
     * @return Returns flag value.
     */
    virtual bool CheckInputOverflowFlag();

    /**
     * Checks whether or not the output overflow flag has been set. This also clears the flag.
     * @return Returns flag value.
     */
    virtual bool CheckOutputOverflowFlag();
};
