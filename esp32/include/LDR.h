#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

class LDR{
    int pin, value;
    int readvalue();
    void LDRTaskinternal();
    public:
    LDR(int pin);
    static void LDRTask(void *param);
    void LDRstartTask();
    int getvalue();

};



#endif