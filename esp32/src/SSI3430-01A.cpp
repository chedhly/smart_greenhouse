#include "SSI3430-01A.h"
bool alert = false;

SSI3430_01A::SSI3430_01A(int P){
    pin = P;
    pinMode(pin, OUTPUT);
}
void SSI3430_01A::control(long x){

    if (x<15){
        digitalWrite(pin, HIGH);
    } 
    else if (x > 25 && !alert){
        alert = true;
        Serial.println("Alert: Water level is too high!");
    }
    else if (x < 10 && !alert){
        alert = true;
        Serial.println("Alert: Water level is too low!");
    }
    else {
        digitalWrite(pin, LOW);
        alert = false;
    }
}