#include <stdlib.h>
#include "stream.h"

namespace{
    inline uint16_t min(uint16_t one, uint16_t two){
        return (one >= two) ? one : two;
    }
}

void Stream::Write(uint8_t *string, uint16_t size) {
	this->WriteToBuffer(this->output_buffer, this->output_buffer_start_index, this->output_buffer_stop_index, this->output_buffer_size, this->output_buffer_empty, this->output_buffer_overflowed, string, size, this->event_output_buffer_not_empty);
}


uint16_t Stream::Read(uint8_t *string, uint16_t size) {
    return this->ReadFromBuffer(this->input_buffer, this->output_buffer_start_index, this->output_buffer_stop_index, this->output_buffer_size, this->input_buffer_empty, string, size);
}

uint8_t Stream::GetAvailableWriteBytes(){
    return this->output_buffer_size - this->CalculateLength(this->output_buffer_start_index, this->output_buffer_stop_index, this->output_buffer_size, this->output_buffer_empty);
}

uint8_t Stream::GetAvailableReadBytes(){
    return this->CalculateLength(this->input_buffer_start_index, this->input_buffer_stop_index, this->input_buffer_size, this->input_buffer_empty);
}

bool Stream::ReadByte(uint8_t& byte){
    return this->ReadByteFromBuffer(byte, this->input_buffer, this->input_buffer_start_index, this->input_buffer_stop_index, this->input_buffer_size, this->input_buffer_empty);
}

void Stream::WriteByte(uint8_t byte){
    this->WriteByteToBuffer(this->output_buffer, this->output_buffer_start_index, this->output_buffer_stop_index, this->output_buffer_size, this->output_buffer_empty, this->output_buffer_overflowed, byte, this->event_output_buffer_not_empty);
}

bool Stream::CheckOutputOverflowFlag(){
    bool flag = this->output_buffer_overflowed;
	this->output_buffer_overflowed = false;
	return flag;
}

uint16_t Stream::ReadFromBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty, uint8_t *string, uint16_t &string_size){
    uint16_t buffer_length = this->CalculateLength(start_index, stop_index, buffer_size, empty);
    uint16_t read_size = min(string_size, buffer_length);

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
    if (read_size == buffer_length) {
        empty = true;
    }
    return read_size;
}

void Stream::WriteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty, bool &overflow_flag, uint8_t *string, uint16_t &string_size, void (*cb)(Stream *stream)){
    uint16_t buffer_available = buffer_size - this->CalculateLength(start_index, stop_index, buffer_size, empty);

    if (string_size > buffer_available) {
        overflow_flag = true;
        start_index = (start_index + (string_size - buffer_available)) % buffer_size;
    }

    // Either the write is on one side of index 0, or on both sides:
    if(((stop_index + 1) % buffer_size) + string_size > buffer_size) {
        // Two sided write
        uint16_t size_one = buffer_size - (stop_index + 1);
        uint16_t size_two = string_size - size_one;
        memcpy(buffer + stop_index + 1, string, size_one);
        memcpy(buffer, string + size_one, size_two);
    } else {
        // One sided write
        memcpy(buffer + ((stop_index + 1) % buffer_size), string, string_size);
    }

    stop_index = (stop_index + string_size) % buffer_size;
    if (string_size > 0 && empty) {
        empty = false;
        if (cb != nullptr) (*cb)(this);
    }
}

bool Stream::ReadByteFromOutputStream(uint8_t &byte) {
	return this->ReadByteFromBuffer(byte, this->output_buffer, this->output_buffer_start_index, this->output_buffer_stop_index, this->output_buffer_size, this->output_buffer_empty);
}

void Stream::WriteByteToInputStream(uint8_t &byte) {
    this->WriteByteToBuffer(this->input_buffer, this->input_buffer_start_index, this->input_buffer_stop_index, this->input_buffer_size, this->input_buffer_empty, this->input_buffer_overflowed, byte, this->event_input_buffer_not_empty);
}
bool Stream::ReadByteFromBuffer(uint8_t& byte,uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty) {
    uint16_t buffer_length = this->CalculateLength(start_index, stop_index, buffer_size, empty);
    if (buffer_length) {
        byte = buffer[start_index];
        start_index = (start_index + 1) % buffer_size;
        if (buffer_length == 1) {
            empty = true;
        }
        return true;
    } else {
        return false;
    }
}

void Stream::WriteByteToBuffer(uint8_t *buffer, uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty, bool &overflow_flag, uint8_t &byte, void (*cb)(Stream *stream)) {
    if (buffer_size - this->CalculateLength(start_index, stop_index, buffer_size, empty) < 1) {
        overflow_flag = true;
        start_index = (start_index + 1) % buffer_size;
    }

    uint16_t to_write = (stop_index + 1) % buffer_size;
    buffer[to_write] = byte;


    stop_index = to_write;
    if (empty) {
        empty = false;
        if (cb != nullptr) (*cb)(this);
    }
}

bool Stream::CheckInputOverflowFlag() {
	bool flag = this->input_buffer_overflowed;
	this->input_buffer_overflowed = false;
	return flag;
}

uint16_t Stream::CalculateLength(uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty) {
    return (empty == true) ? 0 : ((stop_index - start_index) % buffer_size) + 1;
}

uint16_t Stream::ReadFromOutputStream(uint8_t *string, uint16_t size) {
    return this->ReadFromBuffer(this->output_buffer, this->output_buffer_start_index, this->output_buffer_stop_index, this->output_buffer_size, this->output_buffer_empty, string, size);
}

void Stream::WriteToInputStream(uint8_t *string, uint16_t size) {
    this->WriteToBuffer(this->input_buffer, this->input_buffer_start_index, this->input_buffer_stop_index, this->input_buffer_size, this->input_buffer_empty, this->input_buffer_overflowed, string, size, this->event_input_buffer_not_empty);
}

uint16_t Stream::GetInputBufferLength() {
    return this->CalculateLength(this->input_buffer_start_index, this->input_buffer_stop_index, this->input_buffer_size, this->input_buffer_empty);
}

uint16_t Stream::GetOutputBufferLength() {
    return this->CalculateLength(this->output_buffer_start_index, this->output_buffer_stop_index, this->output_buffer_size, this->output_buffer_empty);
}

void Stream::FlushStream(uint16_t &start_index, uint16_t &stop_index, uint16_t &buffer_size, bool &empty){
    stop_index = buffer_size - 1;
    start_index = 0;
    empty = true;
}

void Stream::FlushInputBuffer(){
    this->FlushStream(this->input_buffer_start_index, this->input_buffer_stop_index, this->input_buffer_size,
                      this->input_buffer_empty);
}

void Stream::FlushOutputBuffer(){
    this->FlushStream(this->output_buffer_start_index, this->output_buffer_stop_index, this->output_buffer_size,
                      this->output_buffer_empty);
}

Stream::Stream(uint16_t input_stream_size, uint16_t output_stream_size) : input_buffer_size(input_stream_size),
                                                                          output_buffer_size(output_stream_size) {
    this->input_buffer_stop_index  = this->input_buffer_size - 1;
    this->output_buffer_stop_index = this->output_buffer_size - 1;

    this->input_buffer = (uint8_t *) malloc(this->input_buffer_size);
    this->output_buffer = (uint8_t *) malloc(this->output_buffer_size);
}
