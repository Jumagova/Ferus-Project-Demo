#include <Arduino.h>

#define DIR 21
#define PULL 14

void runMotor(int direction);

void setup()
{
  pinMode(PULL, OUTPUT); // set PULL as PWM output
  pinMode(DIR, OUTPUT);  // set DIR as Direction
}

void loop()
{
  runMotor(HIGH); // run the motor in the clockwise direction at 50 RPM
}

void runMotor(int direction)
{

  int x;
  unsigned long currentMillis = 0;
  const unsigned long period = 1; // set period to 5ms
  int dutyCycle = 0;              // initial duty cycle value

  // int period = 60000 / speed / 400; // calculate the period in milliseconds
  digitalWrite(DIR, direction); // set the direction
  for (int x = 0; x < 400; x++)
  { // repeat 400 times a revolution when setting 400 on driver
    unsigned long currentMillis = millis();
    if (currentMillis % period == 0)
    {                                             // if the period has passed
      int dutyCycle = (dutyCycle == 0) ? 255 : 0; // toggle the duty cycle between 0 and 255
      analogWrite(PULL, dutyCycle);               // set the duty cycle
    }
  }
}


