#include "DJS-1.h"

DJS_1::DJS_1(int pin, float klow, float khigh): pin(pin), klow(klow), khigh(khigh), ECvalue(0.0) {}

float DJS_1::readVoltage() {
    float sum = 0;

    // averaging to reduce noise
    for(int i = 0; i < 10; i++) {
        sum += analogRead(pin);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    float avg = sum / 10.0;

    return avg * 3.3 / 4095.0;
}
float DJS_1::readEC(float temp){
    float voltage = readVoltage();
    float K =klow + (khigh - klow) * (voltage/3.3);
    ECvalue = K * voltage;
    ECvalue = ECvalue / (1.0 + 0.02 * (temp - 25.0));
    return ECvalue;
}

DJS_1_Manager::DJS_1_Manager(DJS_1 *djs1 , DS18B20 *hydrotemp):djs1Sensor(djs1), hydrotempSensor(hydrotemp) {}
void DJS_1_Manager::STARTTask(){
    xTaskCreate(task,"DJS_1 Task",2048,this,1,NULL);
}
void DJS_1_Manager::task(void *param){
    DJS_1_Manager *manager = (DJS_1_Manager *)param;
    manager->taskloop();
}
void DJS_1_Manager::taskloop(){
    while(1){
        hydrotempSensor->updatevalues(); // Ensure we have the latest temperature for compensation
        float temp = hydrotempSensor->getHydTemp();
        float ecValue = djs1Sensor->readEC(temp);  
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.ec = ecValue;    
        xSemaphoreGive(dataMutex);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

