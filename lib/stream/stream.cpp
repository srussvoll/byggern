#include "stream.h"
namespace{
    inline uint16_t min(uint16_t one, uint16_t two){
        return (one >= two) ? one : two;
    }
}

void Stream::Write(uint8_t *string, uint16_t size) {
	this->WriteToBuffer(this->output_stream, this->output_stream_start_index, this->output_stream_stop_index, this->output_stream_size, this->output_stream_overflowed, string, size);
}



void Stream::Read(uint8_t *string, uint16_t size) {
    this->ReadFromBuffer(this->input_stream, this->output_stream_start_index, this->output_stream_stop_index, this->output_stream_size, string, size);
}

uint8_t Stream::GetAvailableWriteBytes(){
    return this->CalculateLength(this->output_stream_start_index, this->output_stream_stop_index, this->output_stream_size);
}

uint8_t Stream::GetAvailableReadBytes(){
    return this->CalculateLength(this->input_stream_start_index, this->input_stream_stop_index, this->input_stream_size);
}

uint8_t Stream::ReadByte(){
    return this->ReadByteFromBuffer(this->input_stream, this->input_stream_start_index, this->input_stream_stop_index, this->input_stream_size);
}

void Stream::WriteByte(uint8_t byte){
    this->WriteByteToBuffer(this->output_stream, this->output_stream_start_index, this->output_stream_stop_index, this->output_stream_size, this->output_stream_overflowed, byte);
}

bool Stream::CheckOutputOverflowFlag(){
    bool flag = this->output_stream_overflowed;
	this->output_stream_overflowed = false;
	return flag;
}

void Stream::ReadFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, uint8_t *string, uint16_t &string_size){
    uint16_t read_size = min(string_size, this->CalculateLength(start_index, stop_index, buffer_size));
    
    // Either the read is on one side of index 0, or on both sides:
    if (start_index + read_size > buffer_size) {
        // Two sided read
        uint16_t read_size_one = buffer_size - start_index;
        uint16_t read_size_two = read_size - read_size_one;
        memcpy(string, buffer + start_index, read_size_one);
        memcpy(string + read_size_one, buffer, read_size_two);
    } else {
        // One sided read
        memcpy(string, buffer + start_index, read_size);
    }
    
    start_index = (start_index + read_size) % buffer_size;
}

void Stream::WriteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &overflow_flag, uint8_t *string, uint16_t &string_size){
    if (string_size > buffer_size - this->CalculateLength(start_index, stop_index, buffer_size)) {
        overflow_flag = true;
    }
    
    // Either the write is on one side of index 0, or on both sides:
    if(stop_index + 1 + string_size > buffer_size) {
        // Two sided write
        uint16_t size_one = buffer_size - (stop_index + 1);
        uint16_t size_two = string_size - size_one;
        memcpy(buffer + stop_index + 1, string, size_one);
        memcpy(buffer, string + size_one, size_two);
    } else {
        // One sided write
        memcpy(buffer + stop_index + 1, string, string_size);
    }
    
    stop_index = (stop_index + string_size) % buffer_size;
}

uint8_t Stream::ReadByteFromOutputStream(){
    return this->ReadByteFromBuffer(this->output_stream, this->output_stream_start_index, this->output_stream_stop_index, this->output_stream_size);
}

void Stream::WriteByteToInputStream(uint8_t byte){
    this->WriteByteToBuffer(this->input_stream, this->input_stream_start_index, this->input_stream_stop_index, this->input_stream_size, this->input_stream_overflowed, byte);
}

uint8_t Stream::ReadByteFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size){
    uint8_t value = buffer[start_index];
    start_index = (start_index + 1) % buffer_size;
    return value;
}

void Stream::WriteByteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &overflow_flag, uint8_t &byte) {
    if (buffer_size - this->CalculateLength(start_index, stop_index, buffer_size) < 1) {
        overflow_flag = true;
    }

    uint16_t to_write = (stop_index + 1) % buffer_size;
    buffer[to_write] = byte;

    stop_index = to_write;
}

bool Stream::CheckInputOverflowFlag() {
	bool flag = this->input_stream_overflowed;
	this->input_stream_overflowed = false;
	return flag;
}

uint16_t Stream::CalculateLength(uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size) {
    return (stop_index - start_index) % buffer_size;
}

void Stream::ReadFromOutputStream(uint8_t *string, uint16_t size) {
    this->ReadFromBuffer(this->output_stream, this->output_stream_start_index, this->output_stream_stop_index, this->output_stream_size, string, size);
}

void Stream::WriteToInputStream(uint8_t *string, uint16_t size) {
    this->WriteToBuffer(this->input_stream, this->input_stream_start_index, this->input_stream_stop_index, this->input_stream_size, this->input_stream_overflowed, string, size);
}
