#pragma once

#include <string.h>
#include <avr/io.h>
/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø, Sondre Russvoll
 * @version 1.0
 *
 * An interface for handling streams with default methods.
 */
class Stream {	
protected:

	/**
	 * Stores the input stream data
	 */
	uint8_t *input_stream = nullptr;
	
	/**
	 * Stores the output stream data
	 */
	uint8_t *output_stream = nullptr;
	
	/**
	 * An index that indicates where in the output stream the next bit is to be written
	 */
	uint16_t input_stream_start_index = 0;
	
	/**
	 * An index that indicates where in the input stream the next bit is to be written
	 */
	uint16_t output_stream_start_index = 0;
	
	/**
     * The size of the current output stream. This has to be set
	 */
	uint16_t output_stream_size = 0;
	
	/**
	 * The size of the current input stream. This has to be set
	 */
	uint16_t input_stream_size = 0;
	
	/**
	* The index indicates where the input ring buffer stops having data
	*/
	uint16_t input_stream_stop_index = 0;
	
	/**
	 * The index indicates where the output ring buffer stops having data
	 */
	uint16_t output_stream_stop_index = 0;
	
	/**
	 * Flag indicating whether the input stream has overflowed or not.
	 */
	bool input_stream_overflowed = false;
	
	/**
	 * Flag indicating whether the output stream has overflowed or not.
	 */
	bool output_stream_overflowed = false;
	
	
	/**
	* Reads a string from the given buffer. 
	* @param buffer Buffer to read from
	* @param start_index The first valid byte of the buffer
	* @param stop_index The last valid byte of the buffer
	* @param size The size of the buffer
	* @param string The string to read into
	* @param length The length of the string
	*/
	void ReadFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &size, uint8_t *string, uint16_t &length);
	/**
	* Writes a string to the given buffer. 
	* @param buffer Buffer to write to
	* @param start_index The first valid byte of the buffer
	* @param stop_index The last valid byte of the buffer
	* @param size The size of the buffer
	* @param string The string to read from
	* @param length The length of the string
	*/
	void WriteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &size, uint8_t *string, bool &overflow_flag, uint16_t &length);
	
	/**
	* Writes a byte to the input stream
	* @param byte The byte to be written
	*/
	void WriteByteToInputStream(uint8_t byte);
	
	/**
	* Reads a byte from the output stream
	*/
	uint8_t ReadByteFromOutputStream();
	
	/**
	* Reads a byte from the given buffer
	* @param buffer The buffer to read from
	* @param start_index The first valid bit of the buffer
	* @param stop_index The last valid bit of the buffer
	* @param size The size of the buffer
	* @return The byte that was read
	*/
	uint8_t ReadByteFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &size);
	
	/**
	* Writes a byte to the buffer
	* @param buffer The buffer to write to
	* @param start_index The first valid bit of the buffer
	* @param stop_index The last valid bit of the buffer
	* @param size The size of the buffer
	* @param byte The byte to be written
	* @param overflow_flag A flag indicated an overflow
	*/
	void WriteByteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &size, uint8_t &byte, bool &overflow_flag);
	
	/**
	* Calculates the length of the readable part of the buffer
	* @param start_index The start index of the buffer
	* @param stop_index The stop index of the buffer
	* @return Length of valid data
	*/
	uint16_t CalculateLength(uint16_t &start_index, uint16_t &stop_index);
	
	/**
	* Updates the start index
	* @param start_index The start index of the buffer
	* @param stop_index The stop index of the buffer
	* @param bytes_read The amount of bytes already read
	*/
	void UpdateStartIndex(uint16_t &start_index, uint16_t &stop_index, uint16_t &bytes_read);
	
	/**
	* Updates the stop index
	* @param start_index The start index of the buffer
	* @param stop_index The stop index of the buffer
	* @param bytes_written The amount of bytes written already
	*/
	void UpdateStopIndex(uint16_t &start_index, uint16_t &stop_index, uint16_t &bytes_written);
	
public:
	/**
	 * The constructor. Needed to initialize stream sizes
	 *
	 */
	Stream(uint8_t input_stream_size, uint8_t output_stream_size) : input_stream_size(input_stream_size),
																	output_stream_size(output_stream_size){};

	/**
	 * Writes the specified data to the output stream.
	 * @param string Input data
	 * @param size Size of the input data
	 */
	virtual void Write(uint8_t *string, uint16_t size);
	
	/**
	 * Reads data from the input stream and stores in the specified data.
	 * @param string Where the data should be stored
	 * @param size Size of the data
	 */
	virtual void Read(uint8_t *string, uint16_t size);
	
	/**
	 * Simply returns the number of bytes available in the output buffer. The output depends on the maximum bytes allocated in the implementation.
	 */
	virtual uint8_t GetAvailableWriteBytes();
	
	/**
	 * Simply returns the number of bytes available for reading (the actual data available for receiving in the buffer)
	 */
	virtual uint8_t GetAvailableReadBytes();
	
	/**
	 * Reads one byte from the input stream
	 */
	virtual uint8_t ReadByte();
		
	/**
	 * Writes one byte to the output stream
	 */
	virtual void WriteByte(uint8_t byte);
	
	/**
	* Checks whether or not the output overflow flag has been set. If it has, it conducts the nessesary procedures to clear out the overflow
	*/
	virtual bool CheckOutputOverflowFlag();
	

};