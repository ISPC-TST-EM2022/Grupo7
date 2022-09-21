#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


int valor,KY=33;                         // Variables enteras, valor donde se almacenaran los datos de los KY´s
int cuenta=0;                          // Variable que realizara el conteo de detección de objetos
int dato;                              // Variable que almacena el dato del conteo

void setup() {
  Serial.begin(9600);                   // Velocidad de comunicación serial a 9600 baudios
  pinMode(KY,INPUT);                    // Se configura sensor KY como entrada 
  } 
  void loop()   { 
    valor=digitalRead(KY);              // Se lee el valor digital del KY y se asigna a valor 
    if(valor==1){                       // Sí el valor es igual a uno ... 
      Serial.println("Libre");          // No hay detección de obstáculo
      }
      else if(valor==0){                // De lo contrario, si el sensor arroja un cero en la entrada digital
        Serial.println("Obstáculo");    // Se detecta obstáculo 
        dato=cuenta++;                  // y cada vez que sea detectado realizara un conteo de el # de veces 
       Serial.print("Veces Detectadas: "); //de detección de objetos
       Serial.println(dato);               //Se imprimirá el # de veces de detección de objetos      
}
delay(500);        // esperara .5 segundos para realizar nuevamente el proceso
}