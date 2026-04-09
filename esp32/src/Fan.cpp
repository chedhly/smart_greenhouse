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
Fan_Manager::Fan_Manager(Fan *fan, DHT22_sensor *dht22): fan(fan), dht22(dht22){}
void Fan_Manager::task(void *param) {
    Fan_Manager *manager = (Fan_Manager *)param;
    manager->taskloop();
}
void Fan_Manager::taskloop() {
    while (true) {
        float temperature = dht22->getTemperature();
        float humidity = dht22->getHumidity();
        if (temperature > 30.0 || humidity < 70.0) {
            fan->on();
        } else {
            fan->off();
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
void Fan_Manager::STARTTask() {
    xTaskCreate(task, "Fan Manager Task", 2048, this, 1, NULL);
}