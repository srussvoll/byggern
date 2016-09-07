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
	uint8_t output_stream_size = 0;
	
	/**
	 * The size of the current input stream. This has to be set
	 */
	uint8_t input_stream_size = 0;
	
	/**
	* The index indicates where the input ring buffer stops having data
	*/
	uint8_t input_stream_stop_index = 0;
	
	/**
	 * The index indicates where the output ring buffer stops having data
	 */
	uint8_t output_stream_stop_index = 0;
	
	/**
	 * Flag indicating whether the input stream has overflowed or not.
	 */
	bool input_stream_overflowed = false;
	
	/**
	 * Flag indicating whether the output stream has overflowed or not.
	 */
	bool output_stream_overflowed = false;
	
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
	virtual void Read(uint8_t *string, size_t size);
	
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
};