#include<Arduino.h>

int x;
unsigned long currentMillis = 0;
const unsigned long period = 5; // set period to 5ms
int dutyCycle = 0; // initial duty cycle value

void runMotor(int direction, int speed);

void setup() {
  pinMode(14, OUTPUT); // set Pin14 as PWM output
  pinMode(21, OUTPUT); // set Pin21 as DIR
}

void loop() {
  runMotor(HIGH, 200); // run the motor in the clockwise direction at 50 RPM
}

void runMotor(int direction, int speed) {
  int period = 60000 / speed / 400; // calculate the period in milliseconds
  digitalWrite(21, direction); // set the direction
  for (int x = 0; x < 400; x++) { // repeat 400 times a revolution when setting 400 on driver
    unsigned long currentMillis = millis();
    if (currentMillis % period == 0) { // if the period has passed
      int dutyCycle = (dutyCycle == 0) ? 255 : 0; // toggle the duty cycle between 0 and 255
      analogWrite(14, dutyCycle); // set the duty cycle
    }
  }
}
