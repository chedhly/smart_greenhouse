#ifndef DS18B20_H
#define DS18B20_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

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

    static void DS18B20Task(void *param);
    void startTask();

    // Getters
    float getTradTemp();
    float getHydTemp();
};

#endif