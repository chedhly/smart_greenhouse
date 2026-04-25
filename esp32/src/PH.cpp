#include"PH.h"


PH::PH(int pin, float calibrationOffset): pin(pin), calibrationOffset(calibrationOffset), phValue(0.0) {}

float PH::readPH() {
    float sum = 0;
    for (int i=0;i<10;i++){
        sum+=analogRead(pin);
        delay(10);
    }
    float voltage = sum / 10.0 * (3.3 / 4095.0);
    phValue= -5.7 * voltage + calibrationOffset;
    return phValue;
}

void PH::PHTaskInternal() {
    while (1) {
        readPH();
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.ph = phValue;
        xSemaphoreGive(dataMutex);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
void PH::PHTask(void *param) {
    PH *phSensor = (PH *)param;
    phSensor->PHTaskInternal();
}
void PH::PHstartTask() {
    xTaskCreate(PHTask, "PH Task", 2048, this, 1, NULL);
}
