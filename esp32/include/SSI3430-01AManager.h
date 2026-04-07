#ifndef SSI3430_01A_MANAGER_H
#define SSI3430_01A_MANAGER_H
#include"SSI3430-01A.h"
#include"HCSR04.h"

class SSI3430_01A_Manager {
    public:
    SSI3430_01A_Manager(SSI3430_01A *valve1, SSI3430_01A *valve2, HCSR04 *US1, HCSR04 *US2);
    void STARTTask();
    private:
    SSI3430_01A *valve1, *valve2;
    HCSR04 *US1, *US2;
    static void task(void *param);
    void taskloop();
};

#endif