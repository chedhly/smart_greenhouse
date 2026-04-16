#ifndef GY302_H
#define GY302_H
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

class GY302{
    int SDA, SCL;
    float value;
    BH1750 lightMeter;
    float readvalue();
    void GY302Taskinternal();
    public:
    GY302(int sda, int scl);
    void begin();
    static void GY302Task(void *param);
    void GY302startTask();
    float getvalue();

};

#endif