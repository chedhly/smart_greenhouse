#include "PH.h"

PH::PH(int pin, float calibrationOffset): pin(pin), calibrationOffset(calibrationOffset), phValue(0.0) {}

float PH::readPH(float temp) {
    float sum = 0;

    // averaging to reduce noise
    for(int i = 0; i < 10; i++) {
        sum += analogRead(pin);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    float voltage = sum / 10.0 * (3.3 / 4095.0);
    phValue = 7 + ((2.5 - voltage) / 0.18) + calibrationOffset;
    phValue = phValue - (0.02 * (temp - 25.0)); // Temperature compensation
    return phValue;
}

PH_Manager::PH_Manager(PH *phSensor, DS18B20 *hydrotempSensor): phSensor(phSensor), hydrotempSensor(hydrotempSensor) {}


void PH_Manager::STARTTask(){
    xTaskCreate(task,"PH Task",2048,this,1,NULL);
}
void PH_Manager::task(void *param){
    PH_Manager *manager = (PH_Manager *)param;
    manager->taskloop();
}
void PH_Manager::taskloop(){
    while(1){
        hydrotempSensor->updatevalues(); // Ensure we have the latest temperature for compensation
        float temp = hydrotempSensor->getHydTemp();
        float phValue = phSensor->readPH(temp);  
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.ph = phValue;    
        xSemaphoreGive(dataMutex);
        vTaskDelay(pdMS_TO_TICKS(3600000)); // Delay for 1 hour
    }
}
