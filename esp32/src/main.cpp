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


const char* ssid = "";          // TODO: fill in your WiFi SSID
const char* password = "";      // TODO: fill in your WiFi password
const char* mqttServer = "";    // TODO: fill in your MQTT broker IP/hostname
const int mqttPort = 1883;

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
DATA lastPublishedData={};
SemaphoreHandle_t dataMutex=nullptr;
uint32_t timestamp=0;
SemaphoreHandle_t timestampMutex=nullptr;
SemaphoreHandle_t dht22ready=nullptr;
SemaphoreHandle_t gy302ready=nullptr;
SemaphoreHandle_t hcsr04ready=nullptr;

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
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Failed to connect to WiFi");
  }
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      delay(2000);
    }
  }
}

bool hasChanged(const DATA& current, const DATA& last) {
  return (current.temperature != last.temperature) ||
         (current.humidity != last.humidity) ||
         (current.tankWlevel != last.tankWlevel) ||
         (current.traddWlevel != last.traddWlevel) ||
         (current.hydrdWlevel != last.hydrdWlevel) ||
         (current.tradtemp != last.tradtemp) ||
         (current.hydrtemp != last.hydrtemp) ||
         (current.light != last.light) ||
         (current.tds != last.tds) ||
         (current.ph != last.ph) ||
         (current.ec != last.ec) ||
         (current.lightStatus != last.lightStatus) ||
         (current.fanStatus != last.fanStatus);
}

void publishSensorData() {
  if (!mqttClient.connected()) return;

  DATA snapshot;

  xSemaphoreTake(timestampMutex, portMAX_DELAY);
  timestamp = millis();
  xSemaphoreGive(timestampMutex);

  xSemaphoreTake(dataMutex, portMAX_DELAY);
  snapshot = sensorData;
  xSemaphoreGive(dataMutex);

  if (!hasChanged(snapshot, lastPublishedData)) {
    return;
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

  if (mqttClient.publish("greenhouse/sensorData", payload)) {
    lastPublishedData = snapshot;
  } else {
    Serial.println("Failed to publish sensor data");
  }
}

void setup() {
  Serial.begin(115200);

  dataMutex = xSemaphoreCreateMutex();
  timestampMutex = xSemaphoreCreateMutex();
  dht22ready = xSemaphoreCreateBinary();
  gy302ready = xSemaphoreCreateBinary();
  hcsr04ready = xSemaphoreCreateBinary();
  setupWiFi();
   mqttClient.setServer(mqttServer, mqttPort);

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
  static unsigned long lastWiFiReconnectAttempt = 0;
  if (WiFi.status() != WL_CONNECTED) {
    unsigned long now = millis();
    if (now - lastWiFiReconnectAttempt >= 10000) {
      Serial.println("WiFi disconnected, reconnecting...");
      setupWiFi();
      lastWiFiReconnectAttempt = now;
    }
  }
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
  publishSensorData();
  delay(100);
}

