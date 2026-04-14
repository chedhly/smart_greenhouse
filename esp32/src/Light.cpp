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
light_manager::light_manager(LDR *ldr, Light *light): ldr(ldr), light(light){}

void light_manager::task(void *param) {
    light_manager *manager = (light_manager *)param;
    manager->taskloop();
}
void light_manager::taskloop() {
    while (true) {
        int value = ldr->getvalue();
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
