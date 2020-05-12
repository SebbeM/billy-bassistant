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
int debug = 2;
const int threshold = 10;

LOLIN_I2C_MOTOR motor;

int SoundIn = A0;
int DigitalIn = D3;
int BODY = MOTOR_CH_A;
int MOUTH = MOTOR_CH_B;

// The setup routine runs once when you press reset:
void setup() {
  Wire.begin();
  if (debug > 0) {
    Serial.begin(baud);
  }

  printDebug(2, "Motor Shield Testing...");
  while (motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) {
    motor.getInfo();
  }
  printDebug(2, "Motor Shield Ready");

  motor.changeFreq(MOTOR_CH_BOTH, 1600);
  motor.changeDuty(MOTOR_CH_BOTH, 100);
}

// Keep track of whether or not movement is already activated
bool talking = false;
bool looking = false;
unsigned long talktime = millis();
unsigned long looktime = millis();

void loop() {
  int sensorValue = analogRead(SoundIn);
  int listening = digitalRead(DigitalIn);
  printDebug(4, "" + sensorValue);

  // Move body when listening signal is received:
  if (millis() - talktime > 500) {
    if (listening == HIGH && !looking) {
      printDebug(3, "Listening");
      looking = true;
      // Move body out
      motor.changeStatus(BODY, MOTOR_STATUS_CW);
      looktime = millis();
    }
    if (listening == LOW && looking) {
      printDebug(3, "Stop listening");
      looking = false;
      // Move body in
      motor.changeStatus(BODY, MOTOR_STATUS_STOP);
    }
  }

  // Move mouth when sound level reaches threshold:
  if (millis() - talktime > 300) {
    if (sensorValue > threshold && !talking) {
      talking = true;
      // Open mouth
      motor.changeStatus(MOUTH, MOTOR_STATUS_CW);
      talktime = millis();
    }
    if (sensorValue < threshold && talking) {
      talking = false;
      // Close mouth
      motor.changeStatus(MOUTH, MOTOR_STATUS_STOP);
      talktime = millis();
    }
  }
}

void printDebug(int level, String s) {
  if (debug >= level) {
    Serial.println(s);
  }
}
