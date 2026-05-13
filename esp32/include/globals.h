//this is a stucture that contain all the sensor vallues that uses semaphore for real time edditing and the configuration of gloal variables and pin numbers for the project
#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

struct DATA
{
  float temperature;
  float humidity;
  float tankWlevel;
  float tradWlevel;
  float hydrdWlevel;
  float tradtemp;
  float hydrdtemp;
  float light;
  float tds;
  float ph;
  float ec;
  bool lightStatus;
  bool fanStatus;
  
};

extern DATA sensorData;
extern DATA lasttimepublished;
extern SemaphoreHandle_t dataMutex;
extern uint32_t timestamp;

extern TaskHandle_t fanTaskHandle;
extern TaskHandle_t lightTaskHandle;
extern TaskHandle_t valveTaskHandle;
extern SemaphoreHandle_t sensorreadmutex;
extern SemaphoreHandle_t ds18b20mutex;
