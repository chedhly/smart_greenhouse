#include "SSI3430-01A.h"
bool alert = false;

SSI3430_01A::SSI3430_01A(int P):
    pin(P){}

void SSI3430_01A::begin(){
    pinMode(pin, OUTPUT);     
    digitalWrite(pin, LOW);
}
void SSI3430_01A::open(){
    digitalWrite(pin, HIGH);
}
void SSI3430_01A::close(){
    digitalWrite(pin, LOW);
}