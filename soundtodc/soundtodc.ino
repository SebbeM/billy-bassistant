/*
  Make a DC Motor Move to Sound.
  This example code is in the public domain.
   Created by Sebastian Magnusson
   Based on soundtodc by Donald Bell, Maker Project Lab (2016)
   and Sound to Servo by Cenk zdemir (2012)
   and DCMotorTest by Adafruit

  For use with the Adafruit Motor Shield v2 
  ---->  http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

bool debug = true;
const int threshold= 5;
int timer;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *bodyMotor = AFMS.getMotor(1); // M1
Adafruit_DCMotor *mouthMotor = AFMS.getMotor(2); // M2

int SoundPinRight = A0; // Analog in 0
int SoundPinLeft = A1; // Analog in 1
int ListeningPin = 7; // Digital in 13
int Led = LED_BUILTIN;

// The setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600); // Set up Serial library at 9600 bps

  if (debug) {
    Serial.println("Starting soundtodc");
  }

  AFMS.begin();  // Create with the default frequency 1.6KHz

  // Set motors to only run at max speed to minimize noise:
  mouthMotor->setSpeed(255);
  bodyMotor->setSpeed(255);
}

void loop() {
  // Read the analog input on sound pins:
  int sensorValue = analogRead(SoundPinRight) + analogRead(SoundPinLeft);
  // Read the digital input on listening pin:
  int listening = digitalRead(ListeningPin);
  // Map it down to the possible range of  movement:
  sensorValue = map(sensorValue,0,1024,0,255);
  
  analogWrite(Led, sensorValue); // Light up LED according to reading
  
  if (listening == HIGH) {
    if (debug) {
      Serial.println("Listening");
    }
    bodyMotor->run(FORWARD);
    timer = 100;
  }
    
  // Move mouth when sound level reaches threshold:
  if (sensorValue > threshold) {
    if (debug) {
      Serial.println(sensorValue);
    }
    mouthMotor->run(FORWARD);
    delay(30); // Smooth out movement
  } else {
    mouthMotor->run(RELEASE);
    if (timer == 0) {
      bodyMotor->run(RELEASE);
    } else {
      timer--;
      delay(1);
    }
  }
}
