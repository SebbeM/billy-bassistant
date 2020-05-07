/*
 * Controls the motors of a Big Mouth Billy Bass to move along
 * to the sound and actions of a virtual assistant.
 *
 * Author: Sebastian Magnusson https://github.com/SebbeM
*/

#include <Arduino.h>
#include <Wire.h>
#include <LOLIN_I2C_MOTOR.h>

int baud = 115200;
bool debug = true;
const int threshold = 10;

LOLIN_I2C_MOTOR motor;

int SoundIn = A0;
int DigitalIn = D3;
int BODY = MOTOR_CH_A;
int MOUTH = MOTOR_CH_B;

// The setup routine runs once when you press reset:
void setup() {
  Wire.begin();
  Serial.begin(baud);
  if (debug) {
    Serial.println("Motor Shield Testing...");
  }

  while (motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) {
    motor.getInfo();
  }
  if (debug) {
    Serial.println("Motor Shield Ready");
  }

  motor.changeFreq(MOTOR_CH_BOTH, 1600);
  motor.changeDuty(MOTOR_CH_BOTH, 100);
  pinMode(DigitalIn, INPUT_PULLUP);
}

// Keep track of whether or not movement is already activated
bool talking = false;
bool looking = false;
int timer = 0;

void loop() {
  int sensorValue = analogRead(SoundIn);
  int listening = digitalRead(DigitalIn);
  if (debug) {
    Serial.println(sensorValue);
  }

  // Move body when listening signal is received:
  if (timer > 1000) {
    if (listening == HIGH && !looking) {
      if (debug) {
        Serial.println("Listening");
      }
      looking = true;
      motor.changeStatus(BODY, MOTOR_STATUS_CW);
      timer = 0;
    }
    if (listening == LOW && looking) {
      if (debug) {
        Serial.println("Stop listening");
      }
      looking = false;
      motor.changeStatus(BODY, MOTOR_STATUS_STOP);
    }
  }

  // Move mouth when sound level reaches threshold:
  if (timer > 100) {
    if (sensorValue > threshold && !talking) {
      talking = true;
      motor.changeStatus(MOUTH, MOTOR_STATUS_CW);
    }
    if (sensorValue < threshold && talking){
      talking = false;
      motor.changeStatus(MOUTH, MOTOR_STATUS_STOP);
    }
  }
  timer++;
}
