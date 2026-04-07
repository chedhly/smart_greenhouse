#ifndef HCSR04_MANAGER_H
#define HCSR04_MANAGER_H
#include "HCSR04.h"

class HCSR04Manager {
    public:
    HCSR04Manager(HCSR04 *sensor1, HCSR04 *sensor2, HCSR04 *sensor3);
    void STARTTask();
    private:
    HCSR04 *US1, *US2, *US3;
    static void task(void *param);
    void taskloop();
};

#endif