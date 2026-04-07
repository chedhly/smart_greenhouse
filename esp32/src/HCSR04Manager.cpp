#include <Arduino.h>
#include "HCSR04Manager.h"

HCSR04Manager::HCSR04Manager(HCSR04 *sensor1, HCSR04 *sensor2, HCSR04 *sensor3) {
    US1 = sensor1;
    US2 = sensor2;
    US3 = sensor3;
}

void HCSR04Manager::task(void *param) {
    HCSR04Manager *manager = (HCSR04Manager *)param;
    manager->taskloop();
}

void HCSR04Manager::taskloop() {
    while(1){
        US1->mesureDistance();
        US2->mesureDistance();  
        US3->mesureDistance();
        vTaskDelay(5*60*1000 / portTICK_PERIOD_MS);
    }
}

void HCSR04Manager::STARTTask() {
    xTaskCreate(task, "HCSR04 Task", 4096 , this, 1, NULL);
}