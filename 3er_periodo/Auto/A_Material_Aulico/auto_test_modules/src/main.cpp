#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "NewPing.h"      // include NewPing library


#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 4     // Digital pin connected to the DHT sensor 
int trigPin = 25;      // trigger pin
int echoPin = 26;      // echo pin
NewPing sonar(trigPin, echoPin);
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

// Motor A
int motor1Pin2 = 16; 
int motor1Pin1 = 17;
// int motor1Enable = 
// Motor B 
int motor2Pin2 = 18;
int motor2Pin1 = 19;
// int motor2Enable =

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 50;    //velocidad de giro 80% (200/255)

void stop();
void move_forward();
void move_backward();
void turn_left();
void turn_rigth();
void turn_180();

void setup() {
    Serial.begin(9600);
    dht.begin();
    sensor_t sensor;
    // sets the pins as outputs:
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    //pinMode(enable1Pin, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);

/*     // Defined in thingProperties.h
    initProperties();

    // Connect to Arduino IoT Cloud
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    
    /*
        The following function allows you to obtain more information
        related to the state of network and IoT Cloud connection and errors
        the higher number the more granular information you’ll get.
        The default is 0 (only errors).
        Maximum is 4
    //
    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo(); */
}

void loop() {
    // ArduinoCloud.update();
    float tiempo = sonar.ping_median(5);
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    float temperatura = event.temperature;
      // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    float humedad = event.relative_humidity;
    float vsound = 331.3+(0.606*temperatura)+(0.0124*humedad);
    float distance = (tiempo/2.0)*vsound/10000; // distance between sensor and target in cm
    
    if(distance > 50) {
        Serial.println("Out of range");
        move_forward();
    }
    else
    {   
        while (distance <=50)
        {
            turn_left();
            float tiempo1 = sonar.ping_median(5);
            distance = (tiempo1/2.0)*vsound/10000;
        }

    }
}

void stop() {
    Serial.println("Motor stopped");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    delay(100);
}

void move_forward() {
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH); 
};

void move_backward() {
    Serial.println("Moving Backwards");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
};

void turn_left( ){
    Serial.println("Turning left");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
};

void turn_rigth() {

};

void turn_180() {

};
