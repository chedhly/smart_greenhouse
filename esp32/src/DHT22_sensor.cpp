#include "DHT22_sensor.h"

DHT22_sensor::DHT22_sensor(int pin):
    pin(pin),
    temp(0.0),
    hum(0.0),
    dht(pin, DHT22){}

void DHT22_sensor::begin(){
    dht.begin();
}
float DHT22_sensor::readTemperature(){
    return dht.readTemperature();

}   
float DHT22_sensor::readHumidity(){
    return dht.readHumidity();
}
void DHT22_sensor::DHT22Taskinternal(){
    while(1){
        xSemaphoreTake(sensorreadmutex, portMAX_DELAY);
        float temperature = readTemperature();
        float humidity = readHumidity();
        xSemaphoreGive(sensorreadmutex);
        if (!isnan(temperature) && !isnan(humidity)) {
            temp = temperature;
            hum = humidity;
        }
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.temperature = temp;
        sensorData.humidity = hum;
        xSemaphoreGive(dataMutex);

        if (fanTaskHandle != nullptr) {
            xTaskNotifyGive(fanTaskHandle);
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
void DHT22_sensor::DHT22Task(void *param){
    DHT22_sensor *sensor = (DHT22_sensor *)param;
    sensor->DHT22Taskinternal();
}
void DHT22_sensor::DHT22startTask(){
    xTaskCreate(DHT22Task, "DHT22 Task", 2048 , this, 1, NULL);
}
float DHT22_sensor::getTemperature(){
    return temp;
}
float DHT22_sensor::getHumidity(){
    return hum;
}