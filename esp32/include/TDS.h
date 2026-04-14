#ifndef TDS_H
#define TDS_H
#include <Arduino.h>
#include "DS18B20.h"

class TDS {
private:
    int pin;
    float value; 
        float readVoltage();
    float readTDS(float temperature);

public:
    TDS(int pin);
    float getTDS();

friend class TDS_Manager;
};

class TDS_Manager {
private:
    TDS *tdsSensor;
    DS18B20 *tempSensor;

    static void task(void *param);
    void taskloop();

public:
    TDS_Manager(TDS *tdsSensor, DS18B20 *tempSensor);
    void startTask();
};
#endif