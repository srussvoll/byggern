// Created by Sondre Vincent Russvoll on 09.09.16.
// Copyright (c) 2016  All rights reserved.

#ifndef BYGGERN_TESTSTREAM_H
#define BYGGERN_TESTSTREAM_H

#include "lib/stream/stream.h"

class testStream : public Stream {
private:
    uint8_t input_stream[10];
    uint8_t output_stream[10];
public:
    testStream() : Stream(10, 10) {};
};


#endif //BYGGERN_TESTSTREAM_H
