//this is the header file of the HCSR04 class, witch is an Ultrasonic Sensor used to determine the fulness of the tank and the water level in the azolla ponds.
#ifndef HCSR04_H
#define HCSR04_H
#include <Arduino.h>

class HCSR04 {
    int trig,echo;
    public:
    HCSR04(int trigPin, int echoPin);
    long getDistance(long x);
    void mesureDistance();

};
#endif