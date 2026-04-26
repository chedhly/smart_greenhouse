//this is the implementation of the ultrasonic sensor HCSR04 class
#include "HCSR04.h"
HCSR04::HCSR04(int trigPin, int echoPin) {
    trig = trigPin;
    echo = echoPin;
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}
float HCSR04::mesureWaterLevel(long x) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    long duration = pulseIn(echo, HIGH);
     waterlevel = x- duration * 0.034 / 2 ;
     return waterlevel;
}

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
        float waterLevel1 = US1->mesureWaterLevel(100);
        float waterLevel2 = US2->mesureWaterLevel(50);
        float waterLevel3 = US3->mesureWaterLevel(50);
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.tankWlevel = waterLevel1;
        sensorData.traddWlevel = waterLevel2;
        sensorData.hydrdWlevel = waterLevel3;
        xSemaphoreGive(dataMutex);
            if (valveTaskHandle != nullptr) {
                xTaskNotifyGive(valveTaskHandle);
            }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void HCSR04Manager::STARTTask() {
    xTaskCreate(task, "HCSR04 Task", 2048 , this, 1, NULL);
}
float HCSR04::getWaterLevel() {
    return waterlevel;
}