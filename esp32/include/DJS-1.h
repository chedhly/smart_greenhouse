#ifndef DJS_1_H
#define DJS_1_H
#include <Arduino.h>
#include "DS18B20.h"

class DJS_1 {
    int pin;
    float ECvalue,klow,khigh;
    float readVoltage();
    float readEC(float temperature);
    public:
    DJS_1(int pin, float klow, float khigh);
    float getEC();
    friend class DJS_1_Manager;
};
class DJS_1_Manager {
    DJS_1 *djs1Sensor;
    DS18B20 *hydrotempSensor;
    static void task(void *param);
    void taskloop();
    public:
    DJS_1_Manager(DJS_1 *djs1Sensor, DS18B20 *hydrotempSensor);
    void STARTTask();
};



#endif