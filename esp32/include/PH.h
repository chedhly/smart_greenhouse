//this is the header file of the ph sensor class
#pragma once
#include <Arduino.h>
#include "DS18B20.h"
#include "globals.h"

class PH {
    int pin;
    float phValue, calibrationOffset;
    float readPH(float temp);

    public:
    PH(int pin, float calibrationOffset);
    friend class PH_Manager;

};
class PH_Manager {
    PH *phSensor;
    DS18B20 *hydrotempSensor;
    static void task(void *param);
    void taskloop();
    public:
    PH_Manager(PH *phSensor, DS18B20 *hydrotempSensor);
    void STARTTask();
};

