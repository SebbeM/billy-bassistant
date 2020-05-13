/*
 * Controls the motors of a Big Mouth Billy Bass to move along
 * to the sound and actions of a virtual assistant.
 *
 * Author: Sebastian Magnusson https://github.com/SebbeM
*/

#include <Arduino.h>
#include <Wire.h>
#include <LOLIN_I2C_MOTOR.h>

const int baud = 115200;
const unsigned char debug = 0; // Debug level. 1: Errors, 2: Setup, 3: Activity, 4: Sensor
const unsigned short threshold = 40; // Lowest sound level to trigger mouth movement

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
unsigned long talkTime = millis(); // Timestamp for mouth movement triggers

void loop() {
  int sensorValue = analogRead(SoundIn);
  int listening = digitalRead(DigitalIn);
  printDebug(4, String(sensorValue));

  // Move body when listening signal is received:
  if (listening == HIGH && !looking) {
    printDebug(3, "Listening");
    looking = true;
    // Move body out
    motor.changeStatus(BODY, MOTOR_STATUS_CW);
  }
  if (listening == LOW && looking) {
    printDebug(3, "Stop listening");
    looking = false;
    // Move body in
    motor.changeStatus(BODY, MOTOR_STATUS_STOP);
  }

  // Open mouth when sound level reaches threshold:
  if (sensorValue > threshold) {
    talkTime = millis() + sensorValue;
    if (!talking) {
      printDebug(3, "Opening mouth at " + String(sensorValue));
      talking = true;
      // Open mouth
      motor.changeStatus(MOUTH, MOTOR_STATUS_CW);
    }
  }
  // Close mouth when sound is under threshold and enough time has passed
  else if (talking && millis() - talkTime > 0) {
    printDebug(3, "Closing mouth at " + String(sensorValue));
    talking = false;
    // Close mouth
    motor.changeStatus(MOUTH, MOTOR_STATUS_STOP);
  }
}

void printDebug(int level, String s) {
  if (debug >= level) {
    Serial.println(s);
  }
}
