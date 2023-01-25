#include <Wire.h>
#include <Zumo32U4.h>

/* Pins:
Pin 5 und 6 unten 
https://www.pololu.com/file/0J864/zumo-32u4-pinout.pdf
*/

Zumo32U4Motors motors;
const int pin13 = 13;
const int pin5 = 5;    // funzt
const int pin6 = 6;    // funzt
const int pin3 = 3;    // funzt aber verkehrt
const int pin2 = 2;    // funzt aber verkehrt
const int pin19 = 19;  // funzt aber verkehrt

int pin13Result = 0;
int pin5Result = 0;
int pin6Result = 0;
int pin3Result = 0;
int pin2Result = 0;
int pin19Result = 0;

int speed = 0;


void setup() {
  pinMode(pin13, INPUT);
  pinMode(pin5, INPUT);
  pinMode(pin6, INPUT);
  pinMode(pin3, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin19, INPUT);

  Serial.begin(9600);
}

void loop() {
  pin13Result = digitalRead(pin13);
  pin5Result = digitalRead(pin5);
  pin6Result = digitalRead(pin6);
  pin3Result = digitalRead(pin3);
  pin2Result = digitalRead(pin2);
  pin19Result = digitalRead(pin19);

  // switch wrong results
  pin2Result = !pin2Result;
  pin3Result = !pin3Result;
  pin19Result = !pin19Result;


  // set the speed of the robi
  if (!pin5Result && !pin6Result) {  // 00
    speed = 0;
  } else if (!pin5Result && pin6Result) {  // 01
    speed = 133;
  } else if (pin5Result && !pin6Result) {  // 10
    speed = 266;
  } else if (pin5Result && pin6Result) {  // 11
    speed = 400;
  }

  speed = 100;

  // set direction of robi
  if (!pin3Result && !pin2Result && !pin19Result) {  // 000
    motors.setSpeeds(speed, speed);
  } else if (!pin3Result && !pin2Result && pin19Result) {  // 001
    motors.setSpeeds(0.8 * speed, speed);
  } else if (!pin3Result && pin2Result && !pin19Result) {  // 010
    motors.setSpeeds(speed, 0.8 * speed); 
  } else if (!pin3Result && pin2Result && pin19Result) {  // 011
    motors.setSpeeds(-speed, speed);
  } else if (pin3Result && !pin2Result && !pin19Result) {  // 100
    motors.setSpeeds(speed, -speed);
  } else if (pin3Result && !pin2Result && pin19Result) {  // 101
    motors.setSpeeds(-0.8 * speed, -speed);
  } else if (pin3Result && pin2Result && !pin19Result) {  // 110
    motors.setSpeeds(-speed, -0.8 * speed);
  } else if (pin3Result && pin2Result && pin19Result) {  // 111
    motors.setSpeeds(-speed, -speed);
  }
}
