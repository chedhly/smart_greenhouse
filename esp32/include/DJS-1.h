//this is the header file of the DJS-1 class, witch is an electric conductivity sensor that determin how much nutrument are in the water pond.
#include <Arduino.h>
#include "DS18B20.h"
#include "globals.h"

class DJS_1 {
    int pin;
    float ECvalue,klow,khigh;
    float readVoltage();
    float readEC(float temperature);
    public:
    DJS_1(int pin, float klow, float khigh);
    friend class DJS_1_Manager;
};
class DJS_1_Manager {
    DJS_1 *djs1Sensor;
    DS18B20 *hydrotempSensor;
    static void task(void *param);
    void taskloop();
    public:
    DJS_1_Manager(DJS_1 *djs1Sensor, DS18B20 *hydrotempSensor);
    void STARTTask();
};



