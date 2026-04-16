#include"Light.h"

Light::Light(int pin): pin(pin){}
void Light::begin(){
    pinMode(pin, OUTPUT);
    off();
}
void Light::on(){
    digitalWrite(pin, HIGH);
}
void Light::off(){
    digitalWrite(pin, LOW);
}
light_manager::light_manager(GY302 *gy302, Light *light): gy302(gy302), light(light){}

void light_manager::task(void *param) {
    light_manager *manager = (light_manager *)param;
    manager->taskloop();
}
void light_manager::taskloop() {
    while (true) {
        int value = gy302->getluxValue();
        if (value == LOW) {
            light->on();
        } else {
            light->off();
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
void light_manager::STARTTask() {
    xTaskCreate(task, "Light Manager Task", 2048, this, 1, NULL);
}
