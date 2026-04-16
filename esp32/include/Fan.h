#pragma once
#include <Arduino.h>
#include "DHT22_sensor.h"

class Fan {
    int pin;
    public:
    Fan(int pin);
    void begin();
    void on();
    void off();
};
class Fan_Manager{
    public:
    Fan_Manager(Fan *fan, DHT22_sensor *dht22);
    void STARTTask();
    private:
    Fan *fan;
    DHT22_sensor *dht22;
    static void task(void *param);
    void taskloop();
};

