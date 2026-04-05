//this is the implementation of the ultrasonic sensor HCSR04 class
#include "HCSR04.h"
HCSR04::HCSR04(int trigPin, int echoPin) {
    trig = trigPin;
    echo = echoPin;
    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}
long HCSR04::getDistance(long x) {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    long duration = pulseIn(echo, HIGH);
    long distance = x- duration * 0.034 / 2 ;
    return distance;
}
