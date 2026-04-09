#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

class LDR{
    int pin;
    void LDRTaskinternal();
    public:
    int readvalue();
    LDR(int pin);
    static void LDRTask(void *param);
    void LDRstartTask();


};



#endif