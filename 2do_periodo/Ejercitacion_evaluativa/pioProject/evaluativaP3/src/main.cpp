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
  if (seguir){
    Serial.println("Seleccione el ejercicio a testear: ");
    Serial.println("1-: Ingrese dos numero y muestre la suma.");
    Serial.println("2-: Preguntar Nombre, Apellido, Direccion y Celular y completar las respuestas.");
    Serial.println("3-: Sumar n numeros ingresados por terminal y calcular su media.");
    Serial.println("4-: Pedir un valor entre 0 y 255 para controlar un diodo led.");
    Serial.println("5-: Ingresar una secuencia de 8 bits que activaran los leds del puerto D.");
    Serial.println("6-: relizar una funcion bool cerradura(tipo) clave");
    seguir = false;
    int dato;
    int control;
    while (Serial.available() == 0){};
       	dato = Serial.parseInt();
      	int suma;
		//Serial.println(dato);
        switch (dato) {
          case 1:
			suma = ejercicio1();
          	Serial.print("El resultado de la suma es: ");
          	Serial.println(suma);
            seguir = true;
          	Serial.println();
          	delay(1000);
            break;
          case 2:
            ejercicio2();
            seguir = true;
            break;
          case 3:
            ejercicio3();
            seguir = true;
            break;
          case 4:          	
          	while (control == 115) {
              ejercicio4();
              Serial.println("Desea ingresar otro valor? s/n");
              while (Serial.available() == 0) {};
              control = Serial.read();
              if (control != 115) {
                break;
              };
            };
            seguir = true;
            break;
          case 5:
            ejercicio5();
          	seguir = true;
            break;
          case 6:
            if (ejercicio6()) {
                digitalWrite(12, HIGH);
                delay(2000);
                digitalWrite(12, LOW);
                delay(500);
            } else {
                Serial.println("Clave incorrecta.");
            };
            
            seguir = true;
            break;
         break;
        };
      
    
  };
};

int ejercicio1() {
  int a, b, res;
  Serial.print("Ingrese el primer numero: ");
  while (Serial.available() == 0){};
  a = Serial.parseInt();
  Serial.println(a);

  Serial.print("Ingrese el segundo numero: ");
  while (Serial.available() == 0){};
  b = Serial.parseInt();
  Serial.println(b);
  
  res = a+b;
  return res;
};


// 2. Preguntar Nombre, Apellido, Direccion y Celular en mensajes diferentes.
// Completar las respuestas.

void ejercicio2() {
  Serial.println("Por favor, a continuacion ingrese los datos solicitados: ");
  Serial.print("Nombre: ");
  while (Serial.available() == 0) {};
  String nombre = Serial.readString();
  Serial.println(nombre);
  Serial.print("Apellido: ");
  while (Serial.available() == 0) {};
  String apellido = Serial.readString();
  Serial.println(apellido);
  Serial.print("Direccion: ");
  while (Serial.available() == 0) {};
  String direccion = Serial.readString();
  Serial.println(direccion);
  Serial.print("Celular: ");
  while (Serial.available() == 0) {};
  String celular = Serial.readString();
  Serial.println(celular);
  Serial.println("Muchas gracias.");
  Serial.println();
};

// 3. Realizar la suma de n números reales ingresados por terminal y calcular su media.

void ejercicio3() {
  Serial.println();
  Serial.print("Por favor, ingrese la cantidad de numeros a ingresar: ");
  while (Serial.available() == 0){};
  int cant = Serial.parseInt();
  float lista_numeros[cant];
  Serial.println(cant);
  Serial.flush();
  for (int i = 0; i < cant; i++) {
    Serial.print("Ingrese un numero: ");
    while (Serial.available() == 0) {};
    lista_numeros[i] = Serial.parseInt();
    Serial.println(lista_numeros[i]);
    Serial.flush();
  };
  float suma = 0;
  for (int i = 0; i < cant; i++) {
    suma += lista_numeros[i];
  };
  float media = suma/cant;
  Serial.println();
  Serial.print("La media de los valores ingresados es: ");
  Serial.println(media);
  
};

