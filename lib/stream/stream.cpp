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



void Stream::Read(uint8_t *string, size_t size) {
	uint16_t read_size = min(size, this->GetAvailableReadBytes());
	
	// Either the read is on one side of index 0, or on both sides:
    if (this->input_stream_start_index + read_size > this->input_stream_size) {
		// One sided read
		memcpy(string, this->input_stream + this->input_stream_start_index, read_size);
	} else {
		// Two sided read
		uint16_t read_size_one = this->input_stream_size - this->input_stream_start_index;
		uint16_t read_size_two = read_size - read_size_one;
		memcpy(string, this->input_stream + this->input_stream_start_index, read_size_one);
		memcpy(string + read_size_one, this->input_stream, read_size_two);
	}
	
	this->input_stream_start_index = (this->input_stream_start_index + read_size) % this->input_stream_size;
}

bool Stream::CheckOutputOverflowFlag(){
	
}