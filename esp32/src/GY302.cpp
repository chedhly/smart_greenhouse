#include "GY302.h"

GY302::GY302(int sda, int scl):
    SDA(sda), SCL(scl){
}

float GY302::readvalue(){
    return lightMeter.readLightLevel();
}

void GY302::GY302Taskinternal(){
    while (true){
        value = readvalue();
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
void GY302::GY302Task(void *p){
    GY302 *gy302 = (GY302 *)p;
    gy302->GY302Taskinternal();
    
}
void GY302 ::GY302startTask(){
    xTaskCreate(GY302Task, "GY302 Task", 2048, this, 1, NULL);
}
float GY302::getvalue(){
    return value;
}
void GY302::begin(){
    Wire.begin(SDA, SCL);
    lightMeter.begin();
}