#pragma once

#include <string.h>
/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø, Sondre Russvoll
 * @version 1.0
 *
 * An interface for handling streams with default methods.
 */
class Stream {
	
public:
	/**
	* Writes the specified data to the output stream.
	* @param string Input data
	* @param size Size of the input data
	*/
	virtual void Write(char* string, size_t size);
	/**
	* Reads data from the input stream and stores in the specified data.
	* @param string Where the data should be stored
	* @param size Size of the data
	*/
	virtual bool Read(char* string, size_t size);
	virtual uint8_t GetAvailableWriteBytes();
	virtual uint8_t GetAvaliableReadBytes();
};