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

// 4. Pedir un valor entre 0 y 255, para controlar el brillo de un diodo led.

void ejercicio4() {
  Serial.println();
  Serial.print("Por favor, ingrese un numero entre 0 y 255 para controlar el brillo del led: ");
  while (Serial.available() == 0) {};
  potencia_led = Serial.parseInt();
  Serial.println(potencia_led);
  if ((0 <= potencia_led) && (potencia_led < 63)) {
    analogWrite(led, potencia_led);
  } else if ((64 <= potencia_led) && (potencia_led < 76)) {
    analogWrite(led, potencia_led);
  } else if ((77 <= potencia_led) && (potencia_led < 127)) {
    analogWrite(led, potencia_led);
  } else if ((128 <= potencia_led) && (potencia_led < 178)) {
    analogWrite(led, potencia_led);
  } else if ((179 <= potencia_led) && (potencia_led < 191)) {
    analogWrite(led, potencia_led);
  } else if ((192 <= potencia_led) && (potencia_led < 229)) {
    analogWrite(led, potencia_led);
  } else if ((230 <= potencia_led) && (potencia_led < 256)) {
    analogWrite(led, potencia_led);
  } else {
    Serial.println("Ingrese un valor entre 0 y 255.");
  };
};

// 5. Ingresar una secuencia de 8 bits por terminal , que activaran los leds
// correspondientes por el puerto D.

void ejercicio5() {
  PORTD = 0b00000000;
  int pot = 0;
  Serial.println("Ingrese una secuancia de 8bits en binario para encender los leds correspondientes.");
  while (Serial.available() == 0) {};
  String dato = Serial.readString();
  Serial.print("La secuencia introducida es: ");
  Serial.println(dato);
  int dato_len = dato.length() + 1; 
  char dato_array[dato_len];
  dato.toCharArray(dato_array, dato_len); 
  for (int i = 0; i < dato_len -1; i++) {
    pot = dato_array[i];
    if (pot == 49) {
      pot = 255;}
    else { pot = 0;};
    
    digitalWrite(i,pot);
    delay(100);
  }
};

// 6. Realizar una función bool cerradura({tipo} clave); que retorne true cuando la clave
// numérica sea 2147483648. Elija el “tipo” de dato apropiadamente. Implemente el
// circuito que active un rele; Utilice el terminal virtual.

bool ejercicio6() {
    while (Serial.available() == 0) {};
    String clave_ingresada = Serial.readString();
    if (clave_ingresada == pass) {
        return true;
    } else {
        return false;
    };
    

};
