#include <Arduino.h>

// int ejercicio1();
// void ejercicio2();
// void ejercicio3();
// void ejercicio4();

bool seguir = true;
int led = 11;
int potencia_led = 0;
int control = 115;
String pass = "2147483648";
int ejercicio1();
void ejercicio2();
void ejercicio3();
void ejercicio4();
void ejercicio5();
bool ejercicio6();




void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(12, OUTPUT);
  DDRD = 0b11111111;

}
void loop() {
  // put your main code here, to run repeatedly:
}
