/*
  Make a DC Motor Move to Sound.
  This example code is in the public domain.
   Created by Sebastian Magnusson
   Based on soundtodc by Donald Bell, Maker Project Lab (2016)
   and Sound to Servo by Cenk zdemir (2012)
   and DCMotorTest by Adafruit
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1 for mouth and M2 for tail
Adafruit_DCMotor *bodyMotor = AFMS.getMotor(1);
Adafruit_DCMotor *mouthMotor = AFMS.getMotor(2);

// Some other Variables we need
int SoundPinRight = A0;
int SoundPinLeft = A1;
int LedPin = 12; //in case you want an LED to activate while mouth moves

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps


  AFMS.begin();  // create with the default frequency 1.6KHz
}

// the loop routine runs over and over again forever:
void loop() {
  uint8_t i;
  
  // read the input on sound pins:
  int sensorValue = (analogRead(SoundPinRight) + analogRead(SoundPinLeft)) / 2;
  // We Map it here down to the possible range of  movement.
  sensorValue = map(sensorValue,0,512,0,255);
  

  // maping the same reading a little bit more down to calculate the time your motor gets
if (sensorValue > 5) { // to cut off some static readings
  Serial.println(sensorValue);
  mouthMotor->setSpeed(255);
  bodyMotor->setSpeed(255);
   delay(1);  // a static delay to smooth things out...
// now move the motor 
   mouthMotor->run(FORWARD);
   delay(50);
  
  analogWrite(LedPin, sensorValue); 
         // and do that move in this delay time

  mouthMotor->run(RELEASE);
  delay(1);
} // Done.
   // turn off the led again.
      analogWrite(LedPin, 0); 
      // and this repeats all the time.
}
