/**
 * @file    ESC_Control .ino
 * @author  Shannonosaurus
 * @brief   This code primes the ESC and adjusts the speed based on potentiometer
 */

#include <Servo.h>
#define ESCPIN 6
#define POTPIN A1      // analog pin used to connect the potentiometer

Servo esc;             // create servo object to control a servo
int16_t val;           // variable to read the value from the analog pin

void setup()
{
  esc.attach(ESCPIN);           // attaches the servo on servopin
  delay(1);
  esc.writeMicroseconds(400);   // primes the ESC so it is ready to go

  Serial.begin(9600);
}

void loop()
{
  val = analogRead(POTPIN);               // reads the value of the potentiometer (value between 0 and 1023)
  val = map(val, 0, 1023, 0, 2000);       // scale it to use it with the servo (value between 0 and 2000)
  esc.writeMicroseconds(val);             // sets the servo position according to the scaled value

  Serial.print("ESC at speed: ");
  Serial.println(val);
  delay(1000);                             // waits for the servo to get there
}
