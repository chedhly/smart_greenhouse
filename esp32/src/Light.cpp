#include"Light.h"

Light::Light(int pin): pin(pin){}
void Light::begin(){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}
void Light::on(){
    digitalWrite(pin, HIGH);
}
void Light::off(){
    digitalWrite(pin, LOW);
}