#pragma once
#include <Arduino.h>
#include "GY302.h"
#include "globals.h"
class Light{
    int pin;
    public:
    Light(int pin);
    void begin();
    void on();
    void off();

};
class light_manager{
    public:
    light_manager(GY302 *gy302, Light *light);
    void STARTTask();
    private:
    bool state;
    GY302 *gy302;
    Light *light;
    static void task(void *param);
    void taskloop();
};


