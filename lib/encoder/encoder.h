#pragma once

class Encoder {
public:
    Encoder();
    int16_t ReadByte();
    void Reset();
};