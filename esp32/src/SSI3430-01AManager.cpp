#include"SSI3430-01AManager.h"
#include <Arduino.h>

SSI3430_01A_Manager::SSI3430_01A_Manager(SSI3430_01A *valve1, SSI3430_01A *valve2, HCSR04 *US1, HCSR04 *US2) {
    this->valve1 = valve1;
    this->valve2 = valve2;
    this->US1 = US1;
    this->US2 = US2;
}
void SSI3430_01A_Manager::task(void *param) {
    SSI3430_01A_Manager *manager = (SSI3430_01A_Manager *)param;
    manager->taskloop();
}
void SSI3430_01A_Manager::taskloop() {
    bool alert1 = false;
    bool alert2 = false;
    while(1){
        long distance1 = US1->getDistance(50);
        long distance2 = US2->getDistance(50);
        if (distance1 <= 20) {
            valve1->open();
        } else if (distance1 < 10 && !alert1) {
            valve1->open();
            alert1 = true;
        }else if (distance1>30 && !alert1)
        {
            valve1->close();
            alert1 = false;
        }
         
        else {
            valve1->close();
        }
        if (distance2 <= 20) {
            valve2->open();
        } else if (distance2 < 10 && !alert2) {
            valve2->open();
            alert2 = true;
        } else if (distance2 > 30 && !alert2) {
            valve2->close();
            alert2 = false;
        } else {
            valve2->close();
        }
        vTaskDelay(5*60*1000 / portTICK_PERIOD_MS);
    }
}
void SSI3430_01A_Manager::STARTTask() {
    xTaskCreate(task, "SSI3430_01A Task", 2048 , this, 2, NULL);
}