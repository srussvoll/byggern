#pragma once

/**
 * \brief Struct defining a CAN Message
 */
struct CanMessage {
    /**
     * The size of the can message
     */
    uint8_t size;

    /**
     * Array containing the data
     */
    uint8_t *data;

    /**
     * Can message id
     */
    uint16_t id;

    /**
     * Constructor
     * @param size The size of the can message
     * @param data Array containing the data
     * @param id Can message id
     */
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