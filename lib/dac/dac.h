#pragma once


class DAC{
private:

    /**
    * A constructor that initializes the DAC
    DAC();

public:

    static DAC& GetInstance(){
        static DAC instance;
        return instance;
    }

    /**
     * Beacause of singleton - makes sure its not copied etc.
     */
    DAC(const DAC&) = delete;

    /**
    * Beacause of singleton - makes sure its not copied etc.
    */
    void operator=(const DAC&) = delete;
};