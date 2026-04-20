//this is the header file of the GY302 class, witch is a sensor that mesure the light intensity in the greenhouse.
#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>
#include "globals.h"

class GY302{
    int SDA, SCL;
    float luxValue;
    BH1750 lightMeter;
    float readluxValue();
    void GY302Taskinternal();
    public:
    GY302(int sda, int scl);
    void begin();
    static void GY302Task(void *param);
    void GY302startTask();
    float getluxValue();

};

