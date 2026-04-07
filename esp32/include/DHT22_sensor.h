#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H
#include <Arduino.h>
#include <DHT.h>

class DHT22_sensor {
    int pin;
    DHT dht;
    float readTemperature();
    float readHumidity();
    void  DHT22Taskinternal();
    public:
    DHT22_sensor(int pin);
    void begin();
    static void DHT22Task(void *param);
    void DHT22startTask();
    
};

#endif