/*
 * Controls the motors of a Big Mouth Billy Bass to move along
 * to the sound and actions of a virtual assistant.
 *
 * Author: Sebastian Magnusson https://github.com/SebbeM
*/

#include <Arduino.h>
#include <LOLIN_I2C_MOTOR.h>

int baud = 115200;
bool debug = false;
const int threshold= 10;
int timer;

LOLIN_I2C_MOTOR motor;

int SoundIn = A0;
int DigitalIn = D3;
int BODY = MOTOR_CH_A;
int MOUTH = MOTOR_CH_B;

// The setup routine runs once when you press reset:
void setup() {
  Serial.begin(baud);
  Serial.println("Motor Shield Testing...");

  while (motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR)
  {
    motor.getInfo();
    Serial.println("Motor Shield Ready");
  }

  motor.changeFreq(MOTOR_CH_BOTH, 1600);
  motor.changeDuty(MOTOR_CH_BOTH, 100);
}

void loop() {
  int sensorValue = analogRead(SoundIn);
  int listening = digitalRead(DigitalIn);

  if (listening == HIGH) {
    if (debug) {
      Serial.println("Listening");
    }
    motor.changeStatus(BODY, MOTOR_STATUS_CW);
    timer = 10;
  } else {
    motor.changeStatus(BODY, MOTOR_STATUS_STOP);
  }

  // Move mouth when sound level reaches threshold:
  if (sensorValue > threshold) {
    if (debug) {
      Serial.println(sensorValue);
    }
    motor.changeStatus(MOUTH, MOTOR_STATUS_CW);
    //delay(1); // Smooth out movement
  } else {
    motor.changeStatus(MOUTH, MOTOR_STATUS_STOP);
  }
}
