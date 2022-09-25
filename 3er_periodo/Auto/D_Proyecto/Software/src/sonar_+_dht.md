#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

#include "NewPing.h"      // include NewPing library

#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 4     // Digital pin connected to the DHT sensor 
// for Arduino microcontroller
int trigPin = 25;      // trigger pin
int echoPin = 26;      // echo pin

// for ESP8266 microcontroller
//int trigPin = D0;      // trigger pin
//int echoPin = D1;      // echo pin

// for ESP32 microcontroller
//int trigPin = 4;      // trigger pin
//int echoPin = 0;      // echo pin

NewPing sonar(trigPin, echoPin);
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

void setup(){
    Serial.begin(9600);
    // Initialize device.
    dht.begin();
    Serial.println(F("DHTxx Unified Sensor Example"));
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
    Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
    Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
    Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
    Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
    Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
    // Set delay between sensor readings based on sensor details.
    delayMS = sensor.min_delay / 1000;
}

void loop(){
    float tiempo = sonar.ping_median(5);  //sonar.ping_median(iterations); - Do multiple 
                                        //pings (default=5), discard out of range pings 
                                        //and return median in microseconds
    // Delay between measurements.
    // delay(delayMS);
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    float temperatura = event.temperature;
      // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    float humedad = event.relative_humidity;
    float vsound = 331.3+(0.606*temperatura)+(0.0124*humedad);
    float distance = (tiempo/2.0)*vsound/10000; // distance between sensor and target in cm
  
    if(distance>400 || distance<2) Serial.println("Out of range");
    else
    {
        Serial.print("Tiempo de eco: ");
        Serial.print(tiempo, 1); Serial.println(" ms");
        Serial.print("Distancia: ");
        Serial.print(distance); Serial.println(" cm");
    }
 
    delay(50);
}