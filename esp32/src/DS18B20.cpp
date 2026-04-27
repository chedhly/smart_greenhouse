#include "DS18B20.h"

// Constructor
DS18B20::DS18B20(int pin)
    : pin(pin), oneWire(pin), sensors(&oneWire),
      tradTempValid(false), hydTempValid(false), pondtrdTemp(0), pondhydTemp(0) {}

// Init
void DS18B20::begin() {
    sensors.begin();

    Serial.print("Number of DS18B20 sensors: ");
    Serial.println(sensors.getDeviceCount());

}

// Read temperatures (internal)
void DS18B20::readWTemperature() {
    sensors.requestTemperatures();

    if (tradTempValid) {
        float t = sensors.getTempC(tradaddress);
        if (t != DEVICE_DISCONNECTED_C) {
            pondtrdTemp = t;
        }
    }

    if (hydTempValid) {
        float t = sensors.getTempC(hydaddress);
        if (t != DEVICE_DISCONNECTED_C) {
            pondhydTemp = t;
        }
    }
}

void DS18B20::updatevalues() {
    xSemaphoreTake(ds18b20mutex, portMAX_DELAY);
    xSemaphoreTake(sensorreadmutex, portMAX_DELAY);
    readWTemperature();
    xSemaphoreGive(sensorreadmutex);
    xSemaphoreGive(ds18b20mutex);

    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sensorData.tradtemp = pondtrdTemp;  
    sensorData.hydrtemp = pondhydTemp;
    xSemaphoreGive(dataMutex);
}
void DS18B20::setaddress(const DeviceAddress& trad, const DeviceAddress& hyd) {
    memcpy(tradaddress, trad, sizeof(DeviceAddress));
    memcpy(hydaddress, hyd, sizeof(DeviceAddress));
    tradTempValid = true;
    hydTempValid = true;
}

// RTOS loop
void DS18B20::DS18B20Taskinternal() {
    while (true) {
        readWTemperature();
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.tradtemp = pondtrdTemp;
        sensorData.hydrtemp = pondhydTemp;
        xSemaphoreGive(dataMutex);

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

// Task wrapper
void DS18B20::DS18B20Task(void *param) {
    DS18B20 *sensor = (DS18B20 *)param;
    sensor->DS18B20Taskinternal();
}

// Start task
void DS18B20::startTask() {
    xTaskCreate(
        DS18B20Task,
        "DS18B20 Task",
        2048,
        this,
        1,
        NULL
    );
}

// Getters
float DS18B20::getTradTemp() {
    return pondtrdTemp;
}

float DS18B20::getHydTemp() {
    return pondhydTemp;
}