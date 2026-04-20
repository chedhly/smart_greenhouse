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
  
  uint32_t timestamp;
}DATA;

extern DATA sensorData;
extern SemaphoreHandle_t dataMutex;
