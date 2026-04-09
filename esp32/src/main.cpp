#include <Arduino.h>
#include "HCSR04.h"
#include "SSI3430-01A.h"
#include "DHT22_sensor.h"
#include "DS18B20.h"
#include "LDR.h"
#include "Light.h"
#include "Fan.h"



const int US1_TRIG_PIN = 25;
const int US1_ECHO_PIN = 26;
const int US2_TRIG_PIN = 27;
const int US2_ECHO_PIN = 18;
const int US3_TRIG_PIN = 21;
const int US3_ECHO_PIN = 22;
const int DHT22_PIN = 16  ;
const int DS18B20_PIN = 17;
const int LDR_PIN = 33;

const int VALVE1_PIN = 13;
const int VALVE2_PIN = 5;
const int lamp_PIN = 19;
const int fan_PIN = 23;


HCSR04 UStank(US1_TRIG_PIN, US1_ECHO_PIN);
HCSR04 USAtrad(US2_TRIG_PIN, US2_ECHO_PIN);
HCSR04 USAhyd(US3_TRIG_PIN, US3_ECHO_PIN);
DHT22_sensor dht22(DHT22_PIN);
DS18B20 ds18b20(DS18B20_PIN);
LDR ldr(LDR_PIN);

SSI3430_01A valve1 (VALVE1_PIN);
SSI3430_01A valve2 (VALVE2_PIN);
Light lamp(lamp_PIN);
Fan fan(fan_PIN);

HCSR04Manager HCSR04manager(&UStank, &USAtrad, &USAhyd);
SSI3430_01A_Manager valveManager(&valve1, &valve2, &USAtrad, &USAhyd);
light_manager lightManager(&ldr, &lamp);
Fan_Manager fanManager(&fan, &dht22);


void setup() {
  Serial.begin(115200);
  dht22.begin();
  valve1.begin();
  valve2.begin();
  ds18b20.begin();
  lamp.begin();
  fan.begin();

  dht22.DHT22startTask();
  ds18b20.startTask();
  ldr.LDRstartTask();
  fanManager.STARTTask();


  HCSR04manager.STARTTask();
  valveManager.STARTTask();
  lightManager.STARTTask();
}

void loop() {
}

