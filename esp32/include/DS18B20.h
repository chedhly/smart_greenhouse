//this is the header file of the DS18B20 class, witch is a sensor that mesure the water tempreture op the ponds.
#pragma once
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "globals.h"

class DS18B20 {
private:
    int pin;
    OneWire oneWire;
    DallasTemperature sensors;

    DeviceAddress sensorAddresses[2];
    int sensorCount;

    // Shared values
    float pondtrdTemp;
    float pondhydTemp;

    void readWTemperature();
    void printAddress(DeviceAddress deviceAddress);
    void DS18B20Taskinternal();

public:
    DS18B20(int pin);

    void begin();
    void updatevalues();
    static void DS18B20Task(void *param);
    void startTask();

    // Getters
    float getTradTemp();
    float getHydTemp();
};
