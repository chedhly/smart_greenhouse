//this is a stucture that contain all the sensor vallues that uses semaphore for real time edditing 
#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>


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
extern DATA lasttimepublished;
extern SemaphoreHandle_t dataMutex;
extern uint32_t timestamp;
extern SemaphoreHandle_t timestampMutex;

extern TaskHandle_t fanTaskHandle;
extern TaskHandle_t lightTaskHandle;
extern TaskHandle_t valveTaskHandle;
extern SemaphoreHandle_t sensorreadmutex;
extern SemaphoreHandle_t ds18b20mutex;
