#include "SSI3430-01A.h"

bool alert1 = false;
bool alert2 = false;

SSI3430_01A::SSI3430_01A(int P):
    pin(P){}

void SSI3430_01A::begin(){
    pinMode(pin, OUTPUT);     
    close();
}
void SSI3430_01A::open(float level){
    digitalWrite(pin, HIGH);
    
}
void SSI3430_01A::close(){
    digitalWrite(pin, LOW);
}
SSI3430_01A_Manager::SSI3430_01A_Manager(SSI3430_01A *valve1, SSI3430_01A *valve2, HCSR04 *US2, HCSR04 *US3) {
    this->valve1 = valve1;
    this->valve2 = valve2;
    this->US2 = US2;
    this->US3 = US3;
    this->TaskHandle = nullptr;
}
void SSI3430_01A_Manager::task(void *param) {
    SSI3430_01A_Manager *manager = (SSI3430_01A_Manager *)param;
    manager->taskloop();
}
void SSI3430_01A_Manager::taskloop() {
    while(1){
        if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY) == 0) {

        long distance1 = US2->getWaterLevel();
        long distance2 = US3->getWaterLevel();
        if (10 < distance1 && distance1 < 20) {
            valve1->open(distance1);
        } else if (distance1 < 10 && !alert1) {
            valve1->open(distance1);
            alert1 = true;
        }else if (distance1>30 && !alert1)
        {
            valve1->close();
            alert1 = false;
        }
         
        else {
            valve1->close();
        }
        if (10<distance2 && distance2 < 20) {
            valve2->open(distance2);
        } else if (distance2 < 10 && !alert2) {
            valve2->open(distance2);
            alert2 = true;
        } else if (distance2 > 30 && !alert2) {
            valve2->close();
            alert2 = false;
        } else {
            valve2->close();
        }
       }
    }
}
void SSI3430_01A_Manager::STARTTask() {
    xTaskCreate(task, "SSI3430_01A Task", 2048 , this, 2, &TaskHandle);
    valveTaskHandle = TaskHandle; // Store the task handle in the global variable
}