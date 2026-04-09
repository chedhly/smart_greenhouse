#ifndef LIGHT_H
#define LIGHT_H
#include <Arduino.h>
class Light{
    int pin;
    public:
    Light(int pin);
    void begin();
    void on();
    void off();

};


#endif