#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H
#include <Arduino.h>
#include <DHT.h>

class DHT22_sensor {
    int pin;
    DHT dht;

    public:
    DHT22_sensor(int pin);
    void begin();
    float readTemperature();
    float readHumidity();
    
};

#endif