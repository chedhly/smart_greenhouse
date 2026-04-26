#include "Fan.h"

Fan::Fan(int pin) : pin(pin) {}

void Fan::begin() {
    pinMode(pin, OUTPUT);
    off();
}
void Fan::on() {
    digitalWrite(pin, HIGH);
}
void Fan::off() {
    digitalWrite(pin, LOW);
}
Fan_Manager::Fan_Manager(Fan *fan, DHT22_sensor *dht22): fan(fan), dht22(dht22), state(false), TaskHandle(nullptr) {}
void Fan_Manager::task(void *param) {
    Fan_Manager *manager = (Fan_Manager *)param;
    manager->taskloop();
}
void Fan_Manager::taskloop() {
    while (true) {
        if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(10000)) == 0) {
            float temperature = dht22->getTemperature();
            float humidity = dht22->getHumidity();
            if (temperature > 30.0 || humidity < 70.0) {
                fan->on();
                state = true;
            }else {
            fan->off();
            state = false;
        }
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sensorData.fanStatus = state;
        xSemaphoreGive(dataMutex);
    }
}
}
void Fan_Manager::STARTTask() {
    xTaskCreate(task, "Fan Manager Task", 2048, this, 1, &TaskHandle);
    fanTaskHandle = TaskHandle; // Store the task handle in the global variable

}