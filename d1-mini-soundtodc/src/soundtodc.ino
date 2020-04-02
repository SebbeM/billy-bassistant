/*
 * Controls the motors of a Big Mouth Billy Bass to move along
 * to the sound and actions of a virtual assistant.
 *
 * Author: Sebastian Magnusson https://github.com/SebbeM
*/

#include <Arduino.h>
#include <LOLIN_I2C_MOTOR.h>

bool debug = true;
const int threshold= 5;
int timer;

LOLIN_I2C_MOTOR motor;

int SoundIn = A0;
int DigitalIn = 3;
int Led = LED_BUILTIN;
int BODY = MOTOR_CH_A;
int MOUTH = MOTOR_CH_B;

// The setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  Serial.println("Motor Shield Testing...");

  while (motor.PRODUCT_ID != PRODUCT_ID_I2C_MOTOR) //wait motor shield ready.
  {
    motor.getInfo();
  }

  if (debug) {
    Serial.println("Starting soundtodc");
  }

  motor.changeFreq(MOTOR_CH_BOTH, 1000); //Change A & B 's Frequency to 1000Hz.
  motor.changeDuty(MOTOR_CH_BOTH, 100);
}

void loop() {
  int sensorValue = analogRead(SoundIn);
  int listening = digitalRead(DigitalIn);
  sensorValue = map(sensorValue,0,1024,0,255);

  analogWrite(Led, sensorValue); // Light up LED according to reading

  if (listening == HIGH) {
    if (debug) {
      Serial.println("Listening");
    }
    motor.changeStatus(BODY, MOTOR_STATUS_CW);
    timer = 100;
  }

  // Move mouth when sound level reaches threshold:
  if (sensorValue > threshold) {
    if (debug) {
      Serial.println(sensorValue);
    }
    motor.changeStatus(MOUTH, MOTOR_STATUS_CCW);
    delay(10); // Smooth out movement
  } else {
    motor.changeStatus(MOUTH, MOTOR_STATUS_STOP);
    if (timer == 0) {
      motor.changeStatus(BODY, MOTOR_STATUS_STOP);
    } else {
      timer--;
      delay(1);
    }
  }
}
