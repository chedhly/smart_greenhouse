//this is the header of the class for the Solenoid valve that control the water flow in the changment of the azolla water in the greenhouse.
#ifndef SSI3430_01A_H
#define SSI3430_01A_H
#include <Arduino.h>

class SSI3430_01A {
    int pin;

    public:
    SSI3430_01A(int pin);
    void begin();
    void open();
    void close();
};


#endif