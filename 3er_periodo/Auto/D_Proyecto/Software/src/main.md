// Include Wire Library for I2C
#include <Arduino.h>
#include <Wire.h>

// Include Adafruit PCA9685 Servo Library
#include <Adafruit_PWMServoDriver.h>

// Creat object to represent PCA9685 at default I2C address
Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40);

// Define maximum and minimum number of "ticks" for the servo motors
// Range from 0 to 4095
// This determines the pulse width

// Min y Max para servos sg90
#define SERVOMIN  80  // Minimum value
#define SERVOMAX  600  // Maximum value

// Define servo motor connections (expand as required)
#define SER0  0   //Servo Motor 0 on connector 0 sg90 US
#define SER1  4  //Servo Motor 1 on connector 4 s3003
#define SER2  8   //Servo Motor 2 on connector 8 sg90

// Variables for Servo Motor positions (expand as required)
int pwm0;
int pwm1;

void setup() {

    // Serial monitor setup
    Serial.begin(115200);

    // Print to monitor
    Serial.println("PCA9685 Servo Test");

    // Initialize PCA9685
    pca9685.begin();

    // Set PWM Frequency to 50Hz
    pca9685.setPWMFreq(50);
    pca9685.setPWM(SER1, 0, 50);

}

void loop() {

 
    // // Move Motor 0 from 0 to 180 degrees
    // for (int posDegrees = 80; posDegrees <= 90; posDegrees+=1) {

    //     // Determine PWM pulse width
    //     pwm0 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
    //     // Write to PCA9685
    //     pca9685.setPWM(SER1, 0, pwm0);
    //     // Print to serial monitor
    //     Serial.print("Motor 0 = ");
    //     Serial.println(posDegrees);
        
    // }
    // pca9685.setPWM(SER1, 0, 101);
    // delay(2000);
    // pca9685.setPWM(SER1, 0, 79);
    // delay(2000);
    
    pca9685.setPWM(SER1, 0, 250);
    delay(1000);
    pca9685.setPWM(SER1, 0, 50);
    delay(1000);

    // for (int posDegrees = 90; posDegrees >= 80; posDegrees-=1) {

    //     // Determine PWM pulse width
    //     pwm0 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
    //     // Write to PCA9685
    //     pca9685.setPWM(SER1, 0, pwm0);
    //     // Print to serial monitor
    //     Serial.print("Motor 0 = ");
    //     Serial.println(posDegrees);
        
    // }
}
  