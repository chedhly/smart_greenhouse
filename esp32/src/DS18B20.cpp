#include "DS18B20.h"


// Array to store sensor addresses
DeviceAddress sensorAddresses[2];
int sensorCount = 0;

DS18B20::DS18B20(int pin):
    pin(pin),
    oneWire(pin),
    sensors(&oneWire) {}

void DS18B20::begin(){
    sensors.begin();
    
    // Discover all sensors on the bus
    sensorCount = sensors.getDeviceCount();
    Serial.print("Number of DS18B20 sensors found: ");
    Serial.println(sensorCount);
    
    // Store addresses of all found sensors
    for (int i = 0; i < sensorCount && i < 2; i++) {
        sensors.getAddress(sensorAddresses[i], i);
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(" address: ");
        printAddress(sensorAddresses[i]);
    }
}

float DS18B20::readWTemperature(){
    // Request temperatures from all sensors on the bus
    sensors.requestTemperatures();
    
    // Read and sum temperatures from all sensors
    float pondtrdTemp = 0.0;
    float pondhydTemp = 0.0;
    float temp[2];
    for (int i = 0; i < sensorCount; i++) {
        temp[i] = sensors.getTempC(sensorAddresses[i]);
    }
    pondtrdTemp = temp[0];
    pondhydTemp = temp[1];
    return (pondhydTemp, pondtrdTemp);
}

void DS18B20::printAddress(DeviceAddress deviceAddress){
    for (int i = 0; i < 8; i++){
        if (deviceAddress[i] < 16) Serial.print("0");
        Serial.print(deviceAddress[i], HEX);
    }
    Serial.println();
}

void DS18B20::DS18B20Taskinternal(){
    while(1){
        float pondhydTemp, pondtrdTemp;
        (pondhydTemp, pondtrdTemp) = readWTemperature();
        Serial.print("Pond Hydro Temperature: ");
        Serial.print(pondhydTemp);
        Serial.println("°C");
        Serial.print("Pond TRD Temperature: ");
        Serial.print(pondtrdTemp);  
        Serial.println("°C");
        
        // Delay for 5 minutes (same as other sensors)
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void DS18B20::DS18B20Task(void *param){
    DS18B20 *sensor = (DS18B20 *)param;
    sensor->DS18B20Taskinternal();
}

void DS18B20::DS18B20startTask(){
    xTaskCreate(DS18B20Task, "DS18B20 Task", 4096, this, 1, NULL);
}
