#pragma once

class SoftwareInterrupt {
private:
    void (*vector)(void)[8] = {nullptr};

    /**
     * Sets up software interrupts.
     */
    SoftwareInterrupt();

public:
    void Trigger(uint8_t vectorNumber);
    void Bind(uint8_t vectorNumber, void (*vector)(void));


public:
    /**
     * Beacause of singleton - makes sure its not copied etc.
     */
    UART(const UART&) = delete;

    /**
    * Beacause of singleton - makes sure its not copied etc.
    */
    void operator=(const UART&) = delete;

    /**
     * A Singleton implementation of this class
     *
     */
    static SoftwareInterrupt& GetInstance(){
        static SoftwareInterrupt instance;
        return instance;
    }
};
