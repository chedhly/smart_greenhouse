#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <math.h>
#include <Wire.h>
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

bool wifiWasConnected = false;
const char* ssid = "Ooredoo 5G_854626"; // TODO: Add your WiFi SSID
const char* password = "7RH6U82J35"; // TODO: Add your WiFi password
const char* mqttServer = "192.168.1.127"; // TODO: Add your MQTT server address
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

const float temperature_threshold = 0.5; // Adjust this threshold based on your requirements
const float humidity_threshold = 2.0; // Adjust this threshold based on your requirements
const float waterlevel_threshold = 1.0; // Adjust this threshold based on your requirements
const float LIGHT_THRESHOLD = 100; // Adjust this threshold based on your requirements
const float TDS_THRESHOLD = 10; // Adjust this threshold based on your requirements
const float PH_THRESHOLD = 0.1; // Adjust this threshold based on your requirements
const float EC_THRESHOLD = 0.1; // Adjust this threshold based on your requirements

DATA sensorData = {};
DATA lasttimepublished = {};

SemaphoreHandle_t dataMutex = nullptr;
SemaphoreHandle_t sensorreadmutex = nullptr;
SemaphoreHandle_t ds18b20mutex = nullptr;

TaskHandle_t fanTaskHandle = nullptr;
TaskHandle_t lightTaskHandle = nullptr;
TaskHandle_t valveTaskHandle = nullptr;

WiFiClient espClient;
PubSubClient mqttClient(espClient);


DeviceAddress trad = {0x28, 0xA5, 0xF7, 0x51, 0x00, 0x00, 0x00, 0x09};
DeviceAddress hyd  = {0x28, 0x5C, 0x19, 0x53, 0x00, 0x00, 0x00, 0x92};

HCSR04 UStank(US1_TRIG_PIN, US1_ECHO_PIN);
HCSR04 USAtrad(US2_TRIG_PIN, US2_ECHO_PIN);
HCSR04 USAhyd(US3_TRIG_PIN, US3_ECHO_PIN);
DHT22_sensor dht22(DHT22_PIN);
DS18B20 ds18b20(DS18B20_PIN);
GY302 gy302(SDA_PIN, SCL_PIN);
TDS tds(TDS_PIN);
PH phSensor(PH_PIN, PH_CALIBRATION_OFFSET);
DJS_1 djs1(DJS1_PIN, klow, khigh);

SSI3430_01A valve1(VALVE1_PIN);
SSI3430_01A valve2(VALVE2_PIN);
Light lamp(lamp_PIN);
Fan fan(fan_PIN);

HCSR04Manager HCSR04manager(&UStank, &USAtrad, &USAhyd);
SSI3430_01A_Manager valveManager(&valve1, &valve2, &USAtrad, &USAhyd);
light_manager lightManager(&gy302, &lamp);
Fan_Manager fanManager(&fan, &dht22);
TDS_Manager tdsManager(&tds, &ds18b20);
DJS_1_Manager djs1Manager(&djs1, &ds18b20);
PH_Manager phManager(&phSensor, &ds18b20);

void setupWiFi() {

  static bool wifiStarted = false;

  if (WiFi.status() == WL_CONNECTED) return;

  if (!wifiStarted) {

    Serial.println("Starting WiFi...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    wifiStarted = true;
  }
}

void printWiFiDebugOnceConnected() {

  if (WiFi.status() != WL_CONNECTED) {
    wifiWasConnected = false;
    return;
  }

  if (wifiWasConnected) return; // print only once per connection

  wifiWasConnected = true;

  Serial.println("\n===== WIFI CONNECTED =====");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("Subnet: ");
  Serial.println(WiFi.subnetMask());

  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  Serial.println("==========================\n");
}

void reconnectMQTT() {
  if (WiFi.status() != WL_CONNECTED) return;
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
  return fabs(a.temperature - b.temperature) > temperature_threshold ||
         fabs(a.humidity - b.humidity) > humidity_threshold ||
         fabs(a.tankWlevel - b.tankWlevel) > waterlevel_threshold ||
         fabs(a.tradWlevel - b.tradWlevel) > waterlevel_threshold ||
         fabs(a.hydrdWlevel - b.hydrdWlevel) > waterlevel_threshold ||
         fabs(a.tradtemp - b.tradtemp) > temperature_threshold ||
         fabs(a.hydrdtemp - b.hydrdtemp) > temperature_threshold ||
         fabs(a.light - b.light) > LIGHT_THRESHOLD ||
         fabs(a.tds - b.tds) > TDS_THRESHOLD ||
         fabs(a.ph - b.ph) > PH_THRESHOLD ||
         fabs(a.ec - b.ec) > EC_THRESHOLD ||
         a.lightStatus != b.lightStatus ||
         a.fanStatus != b.fanStatus;
}


void publishSensorData() {

  if (!mqttClient.connected()) return;

  static unsigned long lastPublish = 0;
  if (millis() - lastPublish < 2000) return;

  DATA snapshot;
  snapshot = sensorData;

  if (!haschanged(snapshot, lasttimepublished)) return;

  char payload[512];

  snprintf(payload, sizeof(payload),
    "{\"temperature\":%.2f,\"humidity\":%.2f,\"tankWlevel\":%.2f,"
    "\"tradWlevel\":%.2f,\"hydrdWlevel\":%.2f,\"tradtemp\":%.2f,"
    "\"hydrdtemp\":%.2f,\"light\":%.2f,\"tds\":%.2f,\"ph\":%.2f,"
    "\"ec\":%.2f,\"lightStatus\":%s,\"fanStatus\":%s,\"timestamp\":%lu}",

    snapshot.temperature,
    snapshot.humidity,
    snapshot.tankWlevel,
    snapshot.tradWlevel,
    snapshot.hydrdWlevel,
    snapshot.tradtemp,
    snapshot.hydrdtemp,
    snapshot.light,
    snapshot.tds,
    snapshot.ph,
    snapshot.ec,
    snapshot.lightStatus ? "true" : "false",
    snapshot.fanStatus ? "true" : "false",
    millis()
  );

  if (mqttClient.publish("greenhouse/sensorData", payload)) {
    Serial.println("MQTT publish success");
    lasttimepublished = snapshot;
    lastPublish = millis();
  } else {
    Serial.print("MQTT publish failed, state=");
    Serial.println(mqttClient.state());
  }
}


void setup() {

  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  dataMutex = xSemaphoreCreateMutex();
  sensorreadmutex = xSemaphoreCreateMutex();
  ds18b20mutex = xSemaphoreCreateMutex();

  if (!dataMutex || !sensorreadmutex || !ds18b20mutex) {
    Serial.println("Mutex creation failed!");
    while (true);
  }
  setupWiFi();
  printWiFiDebugOnceConnected();

  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setBufferSize(512);
  valve1.begin();
  valve2.begin();
  lamp.begin();
  fan.begin();

  dht22.begin();
  ds18b20.begin();
  gy302.begin();

  ds18b20.setaddress(trad, hyd);

  valveManager.STARTTask();
  lightManager.STARTTask();
  fanManager.STARTTask();
  dht22.DHT22startTask();
  ds18b20.startTask();
  gy302.GY302startTask();
  tdsManager.startTask();
  phManager.STARTTask();
  djs1Manager.STARTTask();
  HCSR04manager.STARTTask();
}


void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    setupWiFi();
  }

  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();

  publishSensorData();

}