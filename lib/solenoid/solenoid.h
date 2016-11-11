#pragma once


class Solenoid{
public:
    /**
     * A Singleton implementation of this class
     *
     */
    static Solenoid& GetInstance(){
        static Solenoid instance;
        return instance;
    }

    /**
     * Because of singleton - makes sure its not copied etc.
     */
    Solenoid(const Solenoid&) = delete;

    Solenoid(){};

    void Initialize();

    void Pulse();

private:
};