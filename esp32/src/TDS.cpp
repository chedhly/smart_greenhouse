#include"TDS.h"
#include "globals.h"
TDS::TDS(int pin): pin(pin), value(0.0) {}

float TDS::readVoltage() {
    float sum = 0;

    // averaging to reduce noise
    for(int i = 0; i < 10; i++) {
        sum += analogRead(pin);
        delay(10);
    }

    float avg = sum / 10.0;

    return avg * 3.3 / 4095.0;
}

float TDS::readTDS(float temperature) {
    float voltage = readVoltage();

    float tds = (133.42 * voltage * voltage * voltage
               - 255.86 * voltage * voltage
               + 857.39 * voltage) * 0.5;

    // temperature compensation
    value = tds / (1.0 + 0.02 * (temperature - 25.0));

    return value;
}




TDS_Manager::TDS_Manager(TDS *tdsSensor, DS18B20 *tempSensor) {
    this->tdsSensor = tdsSensor;
    this->tempSensor = tempSensor;
}

void TDS_Manager::startTask() {
    xTaskCreate(task, "TDS Task", 2048, this, 1, NULL);
}

void TDS_Manager::task(void *param) {
    TDS_Manager *manager = (TDS_Manager *)param;
    manager->taskloop();
}

void TDS_Manager::taskloop() {
    while (1) {
        float temperature = tempSensor->getTradTemp();

        float tdsValue = tdsSensor->readTDS(temperature);
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.tds = tdsValue;    
        xSemaphoreGive(dataMutex);      

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}