//this is the header of the class for the Solenoid valve that control the water flow in the changment of the azolla water in the greenhouse.
#pragma once
#include <Arduino.h>
#include "HCSR04.h"
#include <freertos/task.h>

class SSI3430_01A {
    int pin;

    public:
    SSI3430_01A(int pin);
    void begin();
    void open(float l);
    void close();
};
class SSI3430_01A_Manager {
    public:
    SSI3430_01A_Manager(SSI3430_01A *valve1, SSI3430_01A *valve2, HCSR04 *US2, HCSR04 *US3);
    void STARTTask();
    private:
    SSI3430_01A *valve1, *valve2;
    HCSR04 *US2, *US3;
    TaskHandle_t TaskHandle;
    static void task(void *param);
    void taskloop();
};

