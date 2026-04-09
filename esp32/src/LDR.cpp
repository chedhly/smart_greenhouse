#include "LDR.h"

LDR::LDR(int pin):
    pin(pin){}

int LDR::readvalue(){
    return digitalRead(pin);}


void LDR::LDRTaskinternal(){
    while (true){
        int value = readvalue();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
void LDR::LDRTask(void *p){
    LDR *ldr = (LDR *)p;
    ldr->LDRTaskinternal();
    
}
void LDR ::LDRstartTask(){
    xTaskCreate(LDRTask, "LDR Task", 2048, this, 1, NULL);
}
int LDR::getvalue(){
    return value;
}