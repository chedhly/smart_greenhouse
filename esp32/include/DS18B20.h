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
    DeviceAddress tradaddress;
    DeviceAddress hydaddress;
    bool tradTempValid;
    bool hydTempValid;

    // Shared values
    float pondtrdTemp;
    float pondhydTemp;

    void readWTemperature();
    void DS18B20Taskinternal();

public:
    DS18B20(int pin);

    void begin();
    void setaddress(const DeviceAddress& trad, const DeviceAddress& hyd);
    void updatevalues();
    static void DS18B20Task(void *param);
    void startTask();

    // Getters
    float getTradTemp();
    float getHydTemp();
};
