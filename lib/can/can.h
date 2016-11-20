#pragma once

/**
 * \brief Struct defining a CAN Message
 *
 * Defines the CAN_MESSAGE.
 * @param size The size of the can message
 * @param The can data
 * @param The can message id
 */
struct CanMessage {
    uint8_t size;
    uint8_t *data;
    uint16_t id;
    CanMessage(uint8_t size, uint8_t *data, uint16_t id): size(size), data(data), id(id){}
};

/**
 * \brief Purely virtual class defining a CAN interface
 */
class CAN{

protected:
    void (*upper_level)(CanMessage &data);

public:
    /**
     * Sends a can message
     * @param message The CAN_MESSAGE which you are sending
     */
    virtual void SendMessage(CanMessage &message) = 0;

    /**
     * Sets where to send the received can data.
     * @param data The CAN_MESSAGE which should contain the data.
     */
    void SetUpperLevel(void (*handler)(CanMessage &data)) {
        this->upper_level = handler;
    }
};