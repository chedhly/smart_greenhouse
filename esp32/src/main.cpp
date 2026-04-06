#include <Arduino.h>
#include "HCSR04.h"
#include "SSI3430-01A.h"
#include "DHT22_sensor.h"


const int US1_TRIG_PIN = 25;
const int US1_ECHO_PIN = 26;
const int US2_TRIG_PIN = 27;
const int US2_ECHO_PIN = 14;
const int US3_TRIG_PIN = 21;
const int US3_ECHO_PIN = 22;
const int VALVE1_PIN = 13;
const int VALVE2_PIN = 2;
const int DHT22_PIN = 0;

HCSR04 US1(US1_TRIG_PIN, US1_ECHO_PIN);
HCSR04 US2(US2_TRIG_PIN, US2_ECHO_PIN);
HCSR04 US3(US3_TRIG_PIN, US3_ECHO_PIN);
SSI3430_01A valve1 (VALVE1_PIN);
SSI3430_01A valve2 (VALVE2_PIN);
DHT22_sensor dht22(DHT22_PIN);

void setup() {
  Serial.begin(115200);
  dht22.begin();
}

void loop() {
  float temperature = dht22.readTemperature();
  float humidity = dht22.readHumidity();
  long Dtank = US1.getDistance(100);
  long Dazolla_trad = US2.getDistance(50);
  long Dazolla_int = US3.getDistance(50);
  Serial.print("Tank: ");
  Serial.print(Dtank);
  Serial.print(" | Trad: ");
  Serial.print(Dazolla_trad);
  Serial.print(" | Int: ");
  Serial.print(Dazolla_int);
  Serial.print(" | Temperature: ");
  Serial.print(temperature);
  Serial.print(" | Humidity: ");
  Serial.println(humidity);
  valve1.control(Dazolla_trad, "Traditional Azolla");
   valve2.control(Dazolla_int, "Innovative Azolla");
  delay(2000);
}

