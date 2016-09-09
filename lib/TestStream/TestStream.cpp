// Created by Sondre Vincent Russvoll on 09.09.16.
// Copyright (c) 2016  All rights reserved.

#include "TestStream.h"

TestStream::TestStream() : Stream(10, 10) {
    char string[5] = "hei!";
    this->WriteToInputStream((uint8_t*)string, 5);
}