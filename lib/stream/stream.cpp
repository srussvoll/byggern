#include "stream.h"
namespace{
	inline uint16_t min(uint16_t one, uint16_t two){
		return (one >= two) ? one : two;
	}
}

void Stream::Write(uint8_t *string, uint16_t size) {
	if (size > this->GetAvailableWriteBytes()) {
		this->output_stream_overflowed = true;
	}
	
	// Either the write is on one side of index 0, or on both sides:
	if(this->output_stream_stop_index + 1 + size > this->output_stream_size) {
		// One sided write
		memcpy(this->output_stream + this->output_stream_stop_index + 1, string, size);
	} else {
		// Two sided write
		uint16_t size_one = this->output_stream_size - (this->output_stream_stop_index + 1);
		uint16_t size_two = size - size_one;
		memcpy(this->output_stream + this->output_stream_stop_index + 1, string, size_one);
		memcpy(this->output_stream, string + size_one, size_two);
	}
	
	this->output_stream_stop_index = (this->output_stream_stop_index + size) % this->output_stream_size;
}



void Stream::Read(uint8_t *string, uint16_t size) {
	this->ReadFromBuffer(this->input_stream, this->output_stream_start_index, this->output_stream_stop_index, this->output_stream_size, string, size);
}

uint8_t Stream::GetAvailableWriteBytes(){
	
}

uint8_t Stream::GetAvailableReadBytes(){
	
}

uint8_t Stream::ReadByte(){
	
}

void Stream::WriteByte(uint8_t byte){
	
}

bool Stream::CheckOutputOverflowFlag(){
	
}

void Stream::ReadFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &size, uint8_t *string, uint16_t &length){
	uint16_t read_size = min(size, stop_index - start_index);
	
	// Either the read is on one side of index 0, or on both sides:
	if (start_index + read_size > size) {
		// One sided read
		memcpy(string, buffer + start_index, read_size);
		} else {
		// Two sided read
		uint16_t read_size_one = size - start_index;
		uint16_t read_size_two = read_size - read_size_one;
		memcpy(string, buffer + start_index, read_size_one);
		memcpy(string + read_size_one, buffer, read_size_two);
	}
	
	start_index = (start_index + read_size) % size;
}

void Stream::WriteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &size, uint8_t *string, bool &overflow_flag, uint16_t &length){
	if (length > (stop_index - start_index)) {
		overflow_flag = true;
	}
	
	// Either the write is on one side of index 0, or on both sides:
	if(stop_index + 1 + length > size) {
		// One sided write
		memcpy(buffer + stop_index + 1, string, length);
		} else {
		// Two sided write
		uint16_t length_one = size - (stop_index + 1);
		uint16_t length_two = length - length_one;
		memcpy(buffer + stop_index + 1, string, length_one);
		memcpy(buffer, string + length_one, length_two);
	}
	
	stop_index = (stop_index + length) % size;
}

void Stream::WriteByteToInputStream(uint8_t byte){
	
}

uint8_t Stream::ReadByteFromOutputStream(){
	
}

uint8_t Stream::ReadByteFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &size){
	
}

void Stream::WriteByteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &size, uint8_t &byte, bool &overflow_flag){
	
	if(stop_index + 1 == size){ 
		buffer[0] = byte;
	}
}

uint16_t Stream::CalculateLength(uint16_t &start_index, uint16_t &stop_index){
	
}

void Stream::UpdateStartIndex(uint16_t &start_index, uint16_t &stop_index, uint16_t &bytes_read){
	if(((uint32_t)start_index + (uint32_t)bytes_read) >= 0xFFFF){
		start_index %= 0x10000;
		stop_index %= 0x10000;
	}
	start_index += bytes_read;
}

void Stream::UpdateStopIndex(uint16_t &start_index, uint16_t &stop_index, uint16_t &bytes_written){
	if(((uint32_t)stop_index + (uint32_t)bytes_written) >= 0xFFFF){
		start_index %= 0x10000;
		stop_index %= 0x10000;
	}
	stop_index += bytes_written;	
}