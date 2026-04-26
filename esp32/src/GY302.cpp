#include "GY302.h"

GY302::GY302(int sda, int scl):
    SDA(sda), SCL(scl), luxValue(0.0){}

float GY302::readluxValue(){
    return lightMeter.readLightLevel();
}

void GY302::GY302Taskinternal(){
    while (true){
        xSemaphoreTake(sensorreadmutex, portMAX_DELAY);
        luxValue = readluxValue();
        xSemaphoreGive(sensorreadmutex);
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.light = luxValue;
        xSemaphoreGive(dataMutex);

        if (lightTaskHandle != nullptr) {
            xTaskNotifyGive(lightTaskHandle);
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
void GY302::GY302Task(void *p){
    GY302 *gy302 = (GY302 *)p;
    gy302->GY302Taskinternal();
    
}
void GY302 ::GY302startTask(){
    xTaskCreate(GY302Task, "GY302 Task", 2048, this, 1, NULL);
}
float GY302::getluxValue(){
    return luxValue;
}
void GY302::begin(){
    Wire.begin(SDA, SCL);
    lightMeter.begin();
}