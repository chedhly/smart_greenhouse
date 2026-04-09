#include "DS18B20.h"

// Constructor
DS18B20::DS18B20(int pin)
    : pin(pin), oneWire(pin), sensors(&oneWire),
      sensorCount(0), pondtrdTemp(0), pondhydTemp(0) {}

// Init
void DS18B20::begin() {
    sensors.begin();

    sensorCount = sensors.getDeviceCount();

    Serial.print("Number of DS18B20 sensors: ");
    Serial.println(sensorCount);

    for (int i = 0; i < sensorCount && i < 2; i++) {
        if (sensors.getAddress(sensorAddresses[i], i)) {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.print(" address: ");
            printAddress(sensorAddresses[i]);
        }
    }
}

// Read temperatures (internal)
void DS18B20::readWTemperature() {
    sensors.requestTemperatures();

    if (sensorCount >= 1) {
        float t = sensors.getTempC(sensorAddresses[0]);
        if (t != DEVICE_DISCONNECTED_C) {
            pondtrdTemp = t;
        }
    }

    if (sensorCount >= 2) {
        float t = sensors.getTempC(sensorAddresses[1]);
        if (t != DEVICE_DISCONNECTED_C) {
            pondhydTemp = t;
        }
    }
}

// Print address
void DS18B20::printAddress(DeviceAddress deviceAddress) {
    for (int i = 0; i < 8; i++) {
        if (deviceAddress[i] < 16) Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
    Serial.println();
}

// RTOS loop
void DS18B20::DS18B20Taskinternal() {
    while (true) {
        readWTemperature();

        Serial.print("TRD Temp: ");
        Serial.print(pondtrdTemp);
        Serial.print(" °C | HYD Temp: ");
        Serial.println(pondhydTemp);

        vTaskDelay(3000 / portTICK_PERIOD_MS);
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
        4096,
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