//this is a stucture that contain all the sensor vallues that uses semaphore for real time edditing 
#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>


typedef struct
{
  float temperature;
  float humidity;
  float tankWlevel;
  float traddWlevel;
  float hydrdWlevel;
  float tradtemp;
  float hydrtemp;
  float light;
  float tds;
  float ph;
  float ec;
  bool lightStatus;
  bool fanStatus;
  
}DATA;

extern DATA sensorData;
extern DATA lastPublishedData;
extern SemaphoreHandle_t dataMutex;
extern uint32_t timestamp;
extern SemaphoreHandle_t timestampMutex;

extern SemaphoreHandle_t dht22ready;
extern SemaphoreHandle_t gy302ready;
extern SemaphoreHandle_t hcsr04ready;
