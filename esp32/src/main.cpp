#include <Arduino.h>
#include "HCSR04.h"
#include "SSI3430-01A.h"
#include "DHT22_sensor.h"
#include "DS18B20.h"
#include "GY302.h"
#include "Light.h"
#include "Fan.h"
#include "TDS.h"
#include "PH.h"
#include "DJS-1.h"

const float PH_CALIBRATION_OFFSET = 3.5; // Adjust this value based on calibration results
const float klow = 0.5, khigh = 1.5; // Adjust these values based on calibration results for EC sensor

const int US1_TRIG_PIN = 25;
const int US1_ECHO_PIN = 26;
const int US2_TRIG_PIN = 27;
const int US2_ECHO_PIN = 18;
const int US3_TRIG_PIN = 19;
const int US3_ECHO_PIN = 23;
const int DHT22_PIN = 4;
const int DS18B20_PIN = 14;
const int TDS_PIN = 34;
const int PH_PIN = 32;
const int DJS1_PIN=35;
const int SDA_PIN = 21;
const int SCL_PIN = 22;


const int VALVE1_PIN = 13;
const int VALVE2_PIN = 5;
const int lamp_PIN = 2;
const int fan_PIN = 15;


HCSR04 UStank(US1_TRIG_PIN, US1_ECHO_PIN);
HCSR04 USAtrad(US2_TRIG_PIN, US2_ECHO_PIN);
HCSR04 USAhyd(US3_TRIG_PIN, US3_ECHO_PIN);
DHT22_sensor dht22(DHT22_PIN);
DS18B20 ds18b20(DS18B20_PIN);
GY302 gy302(SDA_PIN, SCL_PIN);
TDS tds(TDS_PIN);
PH phSensor(PH_PIN, PH_CALIBRATION_OFFSET);
DJS_1 djs1(DJS1_PIN, klow, khigh);

SSI3430_01A valve1 (VALVE1_PIN);
SSI3430_01A valve2 (VALVE2_PIN);
Light lamp(lamp_PIN);
Fan fan(fan_PIN);

HCSR04Manager HCSR04manager(&UStank, &USAtrad, &USAhyd);
SSI3430_01A_Manager valveManager(&valve1, &valve2, &USAtrad, &USAhyd);
light_manager lightManager(&gy302, &lamp);
Fan_Manager fanManager(&fan, &dht22);
TDS_Manager tdsManager(&tds, &ds18b20);
DJS_1_Manager djs1Manager(&djs1, &ds18b20);


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
  gy302.GY302startTask();
  fanManager.STARTTask();
  tdsManager.startTask();
  phSensor.PHstartTask();

  djs1Manager.STARTTask();
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
  Serial.print(" GY302 Value: ");
  Serial.println(gy302.getvalue());
  Serial.print("TDS Value: ");
  Serial.println(tds.getTDS()); 
  Serial.print("pH Value: ");
  Serial.println(phSensor.getPH());
  Serial.print("EC Value:");
  Serial.println(djs1.getEC());
  delay(3000);

}

