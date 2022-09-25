#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "NewPing.h"      // include NewPing library

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST "https://ag7-w-1c76e-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "ISTythkUSzFXlEBp6mKFbcrnPS9jjZoOxV7M5Zjd"

// los parametros de la red wifi a la que me conecto los seteo en el momento.
#define WIFI_SSID "*****************"
#define WIFI_PASSWORD "***************"

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
// Variables to save date and time
String formattedDate;
String dia;
String dayStamp;
String timeStamp;
//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;
String path = "/esp32";
String user = "chulke - AG7-W";
String sensor = "humedad";

int valuePrueba = 12;


#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN 4     // Digital pin connected to the DHT sensor 

// Min y Max para servos sg90
#define SERVOMIN  80  // Minimum value
#define SERVOMAX  600  // Maximum value
// Define servo motor connections (expand as required)
#define SER0  3   //Servo Motor 0 on connector 0 sg90 US
#define SER1  4  //Servo Motor 1 on connector 4 s3003 (Hombro)
#define SER2  8   //Servo Motor 2 on connector 8 sg90 (Codo)
#define motor1Enable 0
#define motor2Enable 1

# define SDApin 21
# define SCLpin 22

#define PIN_HUM 39

int trigPin = 25;      // trigger pin
int echoPin = 26;      // echo pin
NewPing sonar(trigPin, echoPin);
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

int sensor_obstaculo = 33;

// Motor A
int motor1Pin2 = 16; 
int motor1Pin1 = 17;

// Motor B 
int motor2Pin2 = 18;
int motor2Pin1 = 19;

int pwm_rapido = 4000;
int pwm_medio = 2200;
int pwm_lento = 1800;
int pwm_muylento = 1300;

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 50;    //velocidad de giro 80% (200/255)

int pwm0;
int pwm1;

Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

float dato_humedad = 0;


void stop();
void move_forward(int velocidad);
void move_backward(int velocidad);
void turn_left(int velocidad);
void turn_rigth(int velocidad);
float bajar_brazoSensorH();
void subir_brazoSensorH();
void visor(int posi);
void movimiento_libre();
float radar();
void enviar_datos(float dato_humedad);

void setup() {
    Serial.begin(9600);
    Wire.begin (SDApin, SCLpin);
    pca9685.begin();
    dht.begin();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("conectando a la red wifi");
    while (WiFi.status() != WL_CONNECTED)
        {
            Serial.print(".");
            delay(300);
        }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setwriteSizeLimit(firebaseData, "tiny");

    pca9685.setPWMFreq(50);
    sensor_t sensor;
    // sets the pins as outputs:
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    // pinMode(motor1Enable, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
    // pinMode(motor2Enable, OUTPUT);

    pinMode(PIN_HUM, INPUT);
    pinMode(sensor_obstaculo, INPUT);


    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);

    visor(180);
    

    Serial.print("Testing DC Motor...");
/*  Defined in thingProperties.h        ****para conectarse a arduino cloud****
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
    
    move_forward(pwm_medio);
   
    if(radar() < 50) {
        stop(); // freno el auto
        delay(100);
        int aleatorio = random(100);
        while(radar() < 55){
            Serial.println(radar());
            if (aleatorio < 50) {
                turn_left(pwm_medio);
            }
            else {
                turn_rigth(pwm_medio);
            } 
        } 
           
        move_forward(pwm_medio); 
        
    } 
    int obstaculo = digitalRead(sensor_obstaculo);
    Serial.println(obstaculo);
    bool datos = false;
    if (obstaculo == 0)
    {
        while(obstaculo == 0){
            stop();
            visor(90);
            delay(100);
            if (radar() < 25) {
                Serial.println("Bajando Brazo");
                dato_humedad = bajar_brazoSensorH();
                Serial.println("Subiendo Brazo");
                subir_brazoSensorH();
                datos = true;
                enviar_datos(dato_humedad);
            } else {
                move_backward(pwm_medio);
                delay(100);
            }
            obstaculo = digitalRead(sensor_obstaculo);
        } 
        visor(180);           
    }
    

}

void enviar_datos(float dato_humedad) {
    Serial.println(dato_humedad);
    timeClient.forceUpdate();
    formattedDate = timeClient.getFormattedTime();
    dia = timeClient.getDay();
    json.clear().add("Humedad", dato_humedad);
    json.add("Dia", dia);
    json.add("Hora", formattedDate);
    if (Firebase.pushJSON(firebaseData, path + "/" + user + "/" + sensor, json))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.print("PUSH NAME: ");
        Serial.println(firebaseData.pushName());
        Serial.println("ETag: " + firebaseData.ETag());
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
    
}

float radar() {
    float tiempo = sonar.ping_median(5);
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    float temperatura = event.temperature;
      // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    float humedad = event.relative_humidity;
    float vsound = 331.3+(0.606*temperatura)+(0.0124*humedad);
    float distance = (tiempo/2.0)*vsound/10000; // distance between sensor and target in cm
    return distance;
}

// funcion de movimiento libre a implementar en el futuro.

/* void movimiento_libre() {
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
    
    if(distance > 30) {
        Serial.println("Via libre");
        move_forward();
    }
    else
    {   
        int aleatorio = random(100);
        while (distance <= 30)
        {
            if (aleatorio < 50) {
                turn_left();
            } else {
                turn_rigth();
            }
            float tiempo1 = sonar.ping_median(5);
            distance = (tiempo1/2.0)*vsound/10000;
        }

    }
}*/

void stop() {
    Serial.println("Motor stopped");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    delay(100);
}

void move_forward(int velocidad) {
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    pca9685.setPWM(motor1Enable, 0, velocidad);
    pca9685.setPWM(motor2Enable, 0, velocidad);
};

void move_backward(int velocidad) {
    Serial.println("Moving Backwards");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    pca9685.setPWM(motor1Enable, 0, velocidad);
    pca9685.setPWM(motor2Enable, 0, velocidad);
};

void turn_left(int velocidad) {
    Serial.println("Turning left");
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW); 
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    pca9685.setPWM(motor1Enable, 0, velocidad);
    pca9685.setPWM(motor2Enable, 0, velocidad);
};

void turn_rigth(int velocidad) {
    Serial.println("Turning left");
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH); 
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    pca9685.setPWM(motor1Enable, 0, velocidad);
    pca9685.setPWM(motor2Enable, 0, velocidad);

};

float bajar_brazoSensorH() {
    // Move Motor 1 
    pca9685.setPWM(SER1, 0, 270);

    for (int posDegrees = 180; posDegrees >= 60; posDegrees-=5) {

        // Determine PWM pulse width
        pwm1 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
        // Write to PCA9685
        pca9685.setPWM(SER2, 0, pwm1);
        // Print to serial monitor
        Serial.print("Motor 1 = ");
        Serial.println(posDegrees);
        
        delay(30);
        
    }
    delay(10000);
    float humedad_suelo = analogRead(PIN_HUM);
    Serial.println(humedad_suelo);
    for (size_t i = 0; i < 99; i++)
    {
        humedad_suelo += analogRead(PIN_HUM);
    }
    float prom_hum = humedad_suelo/100;
    Serial.println(prom_hum);
    return prom_hum;
}

void subir_brazoSensorH() {

    pca9685.setPWM(SER1, 0, 90);

    for (int posDegrees = 60; posDegrees <= 180; posDegrees+=90) {

        // Determine PWM pulse width
        pwm1 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
        // Write to PCA9685
        pca9685.setPWM(SER2, 0, pwm1);
        // Print to serial monitor
        Serial.print("Motor 1 = ");
        Serial.println(posDegrees);
        delay(30);
    }
}

void visor(int posi) {
    if (posi == 90)
    {
        for (int posDegrees = 90; posDegrees <= 180; posDegrees+=5) {
        // Determine PWM pulse width
            pwm0 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
            // Write to PCA9685
            pca9685.setPWM(SER0, 0, pwm0);
            delay(30);
        }
    } else if (posi == 180)
    {
        for (int posDegrees = 180; posDegrees >= 90; posDegrees-=5) {
            // Determine PWM pulse width
            pwm0 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
            // Write to PCA9685
            pca9685.setPWM(SER0, 0, pwm0);
            delay(30);
        }
    }
}
      
