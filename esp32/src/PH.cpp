#include"PH.h"


PH::PH(int pin, float calibrationOffset): pin(pin), calibrationOffset(calibrationOffset) {}

float PH::readPH() {
    float voltage = analogRead(pin) * (3.3 /4095.0);
    phValue= -5.7 * voltage + calibrationOffset;
    return phValue;
}

void PH::PHTaskInternal() {
    while (1) {
        readPH();
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.ph = phValue;
        sensorData.timestamp = millis();
        xSemaphoreGive(dataMutex);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
void PH::PHTask(void *param) {
    PH *phSensor = (PH *)param;
    phSensor->PHTaskInternal();
}
void PH::PHstartTask() {
    xTaskCreate(PHTask, "PH Task", 4096, this, 1, NULL);
}
