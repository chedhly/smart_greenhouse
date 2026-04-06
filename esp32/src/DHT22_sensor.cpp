#include "DHT22_sensor.h"

DHT22_sensor::DHT22_sensor(int pin):
    pin(pin),
    dht(pin, DHT22){}

void DHT22_sensor::begin(){
    dht.begin();
}
float DHT22_sensor::readTemperature(){
    return dht.readTemperature();

}   
float DHT22_sensor::readHumidity(){
    return dht.readHumidity();
}
