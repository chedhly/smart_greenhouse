//this is the header file of the ph sensor class
#pragma once
#include <Arduino.h>
#include "globals.h"

class PH {
    int pin;
    float phValue, calibrationOffset;
    float readPH();
    void PHTaskInternal();
    public:
    PH(int pin, float calibrationOffset);
    static void PHTask(void *param);
    void PHstartTask();

};

