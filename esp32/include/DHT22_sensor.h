#pragma once
#include <Arduino.h>
#include <DHT.h>
#include "globals.h"

class DHT22_sensor {
    int pin;
    DHT dht;
    float temp , hum;
    float readTemperature();
    float readHumidity();
    void  DHT22Taskinternal();
    public:
    DHT22_sensor(int pin);
    void begin();
    static void DHT22Task(void *param);
    void DHT22startTask();
    float getTemperature();
    float getHumidity();
    
};

