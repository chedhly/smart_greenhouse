#ifndef DS18B20_H
#define DS18B20_H
#include <OneWire.h>
#include <DallasTemperature.h>  
#include <Arduino.h>

class DS18B20 {
    int pin;
    OneWire oneWire;
    DallasTemperature sensors;
    float readWTemperature();
    void DS18B20Taskinternal();
    void printAddress(DeviceAddress deviceAddress);
    public:
    DS18B20(int pin);
    void begin();
    static void DS18B20Task(void *param);
    void DS18B20startTask();
};

#endif