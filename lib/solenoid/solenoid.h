#pragma once

/**
 * \brief Interface for handling a Solenoid circuit.
 *
 */
class Solenoid {
public:
    /**
     * A Singleton implementation of this class
     *
     */
    static Solenoid & GetInstance(){
        static Solenoid instance;
        return instance;
    }

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    Solenoid(const Solenoid &) = delete;

    /**
     * Initializer. Not used because of singleton
     */
    Solenoid(){};

    /**
     * Initializes the Solenoid. Must be called in order for it to function.
     */
    void Initialize();

    /**
     * Sends a pulse to the Solenoid. Aka triggers it.
     */
    void Pulse();
};