#pragma once

struct CAN_MESSAGE{
    uint8_t size;
    uint8_t *data;
    uint16_t id;
    CAN_MESSAGE(uint8_t size, uint8_t *data, uint16_t id): size(size), data(data), id(id){}
};
class CAN{
public:
    virtual void SendMessage(CAN_MESSAGE &message) = 0;

    void SetUpperLevel(void (*handler)(CAN_MESSAGE &data)) {
        this->upper_level = handler;
    }

protected:
    void (*upper_level)(CAN_MESSAGE &data);
};