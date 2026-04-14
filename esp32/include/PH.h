#ifndef PH_H
#define PH_H
#include <Arduino.h>

class PH {
    int pin;
    float phValue, calibrationOffset;
    float readPH();
    void PHTaskInternal();
    public:
    PH(int pin, float calibrationOffset);
    static void PHTask(void *param);
    void PHstartTask();
    float getPH();

};

#endif