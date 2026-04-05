#ifndef HCSR04_H
#define HCSR04_H
#include <Arduino.h>

class HCSR04 {
    int trig;
    int echo;

    public:
    HCSR04(int trigPin, int echoPin);
    long getDistance();
    
};
#endif