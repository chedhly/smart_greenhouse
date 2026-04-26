#include <Arduino.h>
#include "globals.h"
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
#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid = ""; // TODO: Add your WiFi SSID
const char* password = ""; // TODO: Add your WiFi password
const char* mqttServer = ""; // TODO: Add your MQTT server address
const int mqttPort = 1883; // TODO: Add your MQTT server port

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
const int lamp_PIN = 16;
const int fan_PIN = 17;

DATA sensorData={};
DATA lasttimepublished={};
SemaphoreHandle_t dataMutex=nullptr;
uint32_t timestamp=0;
SemaphoreHandle_t timestampMutex=nullptr;
TaskHandle_t fanTaskHandle = nullptr;
TaskHandle_t lightTaskHandle = nullptr; 
TaskHandle_t valveTaskHandle = nullptr;
SemaphoreHandle_t sensorreadmutex = nullptr;
SemaphoreHandle_t ds18b20mutex = nullptr;

WiFiClient espClient;
PubSubClient mqttClient(espClient);


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

void setupWiFi() {
  static unsigned long lastAttempt= 0;
  unsigned long currentTime = millis();
  if (currentTime - lastAttempt < 5000) return; // Only attempt to connect every 5 seconds
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status()!=WL_CONNECTED && attempts < 20)
  {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status()==WL_CONNECTED)
  {
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Failed to connect to WiFi");
  }
  lastAttempt = currentTime;

}

void reconnectMQTT() {
  static unsigned long lastAttemptTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastAttemptTime < 5000) return;

  if (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT server...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Connected to MQTT server");
    } else {
      Serial.print("Failed to connect to MQTT server, rc=");
      Serial.println(mqttClient.state());
    }
    lastAttemptTime = currentTime;
  }
}
bool haschanged(const DATA& a, const DATA& b) {
  return a.temperature != b.temperature ||
         a.humidity != b.humidity ||
         a.tankWlevel != b.tankWlevel ||
         a.traddWlevel != b.traddWlevel ||
         a.hydrdWlevel != b.hydrdWlevel ||
         a.tradtemp != b.tradtemp ||
         a.hydrtemp != b.hydrtemp ||
         a.light != b.light ||
         a.tds != b.tds ||
         a.ph != b.ph ||
         a.ec != b.ec ||
         a.lightStatus != b.lightStatus ||
         a.fanStatus != b.fanStatus;
}

void publishSensorData() {
  if (!mqttClient.connected())return;
    
  DATA snapshot;

  xSemaphoreTake(timestampMutex, portMAX_DELAY);
  timestamp = millis();
  xSemaphoreGive(timestampMutex);

  xSemaphoreTake(dataMutex, portMAX_DELAY);
  snapshot = sensorData;
  xSemaphoreGive(dataMutex);

  if (!haschanged(snapshot, lasttimepublished)) {
    return; // No significant change, skip publishing
  }
  char payload[512];
  snprintf(payload, sizeof(payload), 
           "{\"temperature\": %.2f, \"humidity\": %.2f, \"tankWlevel\": %.2f, \"traddWlevel\": %.2f, \"hydrdWlevel\": %.2f, \"tradtemp\": %.2f, \"hydrtemp\": %.2f, \"light\": %.2f, \"tds\": %.2f, \"ph\": %.2f, \"ec\": %.2f, \"lightStatus\": %s, \"fanStatus\": %s, \"timestamp\": %lu}",
            snapshot.temperature,
            snapshot.humidity,
            snapshot.tankWlevel,
            snapshot.traddWlevel,
            snapshot.hydrdWlevel,
            snapshot.tradtemp,
            snapshot.hydrtemp,
            snapshot.light,
            snapshot.tds,
            snapshot.ph,
            snapshot.ec,
            snapshot.lightStatus ? "true" : "false",
            snapshot.fanStatus ? "true" : "false",
            timestamp);
  if(mqttClient.publish("greenhouse/sensorData", payload)){
    lasttimepublished = snapshot; // Update last published data only on successful publish
  }else{
    Serial.println("Failed to publish MQTT message");
  }
}

void setup() {
  Serial.begin(115200);

  dataMutex = xSemaphoreCreateMutex();
  timestampMutex = xSemaphoreCreateMutex();
  sensorreadmutex = xSemaphoreCreateMutex();
  ds18b20mutex = xSemaphoreCreateMutex();
  
  setupWiFi();
   mqttClient.setServer(mqttServer, mqttPort);

  dht22.begin();
  valve1.begin();
  valve2.begin();
  ds18b20.begin();
  gy302.begin();
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

  static unsigned long lastserrialprint = 0;
  unsigned long currentTime = millis();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, attempting to reconnect...");
    setupWiFi();
  }
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
  publishSensorData();
  if (currentTime - lastserrialprint >= 5000) {

  
DATA snapshot;
xSemaphoreTake(dataMutex, portMAX_DELAY);
snapshot = sensorData;
xSemaphoreGive(dataMutex);
Serial.printf(
  "T: %.2fC | H: %.2f%% | Tank: %.2f | Trad: %.2f | Hydr: %.2f | TradT: %.2fC | HydrT: %.2fC | Light: %.2f | TDS: %.2f | pH: %.2f | EC: %.2f | Lamp: %s | Fan: %s\n",
  sensorData.temperature,
  sensorData.humidity,
  sensorData.tankWlevel,
  sensorData.traddWlevel,
  sensorData.hydrdWlevel,
  sensorData.tradtemp,
  sensorData.hydrtemp,
  sensorData.light,
  sensorData.tds,
  sensorData.ph,
  sensorData.ec,
  sensorData.lightStatus ? "ON" : "OFF",
  sensorData.fanStatus ? "ON" : "OFF"
);
lastserrialprint = currentTime;
  }


}

