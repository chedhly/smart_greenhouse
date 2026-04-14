#include <Arduino.h>
#include "HCSR04.h"
#include "SSI3430-01A.h"
#include "DHT22_sensor.h"
#include "DS18B20.h"
#include "LDR.h"
#include "Light.h"
#include "Fan.h"
#include "TDS.h"
#include "PH.h"

const float PH_CALIBRATION_OFFSET = 3.5; // Adjust this value based on calibration results

const int US1_TRIG_PIN = 25;
const int US1_ECHO_PIN = 26;
const int US2_TRIG_PIN = 27;
const int US2_ECHO_PIN = 18;
const int US3_TRIG_PIN = 21;
const int US3_ECHO_PIN = 22;
const int DHT22_PIN = 16  ;
const int DS18B20_PIN = 17;
const int LDR_PIN = 33;
const int TDS_PIN = 34;
const int PH_PIN = 32;

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
TDS tds(TDS_PIN);
PH phSensor(PH_PIN, PH_CALIBRATION_OFFSET);

SSI3430_01A valve1 (VALVE1_PIN);
SSI3430_01A valve2 (VALVE2_PIN);
Light lamp(lamp_PIN);
Fan fan(fan_PIN);

HCSR04Manager HCSR04manager(&UStank, &USAtrad, &USAhyd);
SSI3430_01A_Manager valveManager(&valve1, &valve2, &USAtrad, &USAhyd);
light_manager lightManager(&ldr, &lamp);
Fan_Manager fanManager(&fan, &dht22);
TDS_Manager tdsManager(&tds, &ds18b20);


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
  tdsManager.startTask();
  phSensor.PHstartTask();


  HCSR04manager.STARTTask();
  valveManager.STARTTask();
  lightManager.STARTTask();
}

void loop() {

  Serial.print("Temperature: ");
  Serial.print(dht22.getTemperature());
  Serial.print(" °C, Humidity: ");
  Serial.print(dht22.getHumidity());
  Serial.print(" %, TradTemp: ");
  Serial.print(ds18b20.getTradTemp());
  Serial.print(" °C, HydTemp: ");
  Serial.println(ds18b20.getHydTemp());
  Serial.print(" LDR Value: ");
  Serial.println(ldr.getvalue());
  Serial.print("TDS Value: ");
  Serial.println(tds.getTDS()); 
  Serial.print("pH Value: ");
  Serial.println(phSensor.getPH());
  delay(3000);

}

