#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H
#include "LDR.h"
#include "Light.h"

class light_manager{
    public:
    light_manager(LDR *ldr, Light *light);
    void STARTTask();
    private:
    LDR *ldr;
    Light *light;
    static void task(void *param);
    void taskloop();
};


#endif