//this is the header file of the HCSR04 class, witch is an Ultrasonic Sensor used to determine the fulness of the tank and the water level in the azolla ponds.
#ifndef HCSR04_H
#define HCSR04_H
#include <Arduino.h>

class HCSR04 {
    int trig,echo;
    float waterlevel;
    void mesureWaterLevel(long x);

    public:
    HCSR04(int trigPin, int echoPin);
    float getWaterLevel();
friend class HCSR04Manager;

};

class HCSR04Manager {
    public:
    HCSR04Manager(HCSR04 *sensor1, HCSR04 *sensor2, HCSR04 *sensor3);
    void STARTTask();
    private:
    HCSR04 *US1, *US2, *US3;
    static void task(void *param);
    void taskloop();
};
#endif