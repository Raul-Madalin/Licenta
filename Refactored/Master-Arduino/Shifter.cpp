#include "Shifter.h"
#include <Arduino.h>

Shifter::Shifter(int serPin, int srclkPin, int rclkPin) {
  SER = serPin;
  SRCLK = srclkPin;
  RCLK = rclkPin;
}

void Shifter::Load(unsigned char shifter_0, unsigned char shifter_1) {
  digitalWrite(RCLK, LOW);
  shiftOut(SER, SRCLK, MSBFIRST, shifter_0);
  shiftOut(SER, SRCLK, MSBFIRST, shifter_1);
  digitalWrite(RCLK, HIGH);
}

void Shifter::Reset(unsigned char shifter_0, unsigned char shifter_1) {
  shifter_0 = 0;
  shifter_1 = 0;
  Load(shifter_0, shifter_1);
}

// TODO: change stepper to string after motor class implemetation
void Shifter::Mapping(unsigned char shifter_0, unsigned char shifter_1, Motors motor, int dir) {
  switch (motor) {
    case MotorX:
      if (dir == 0)
        bitClear(shifter_1, 0);
      if (dir == 1)
        bitSet(shifter_1, 0);
      break;
    case MotorY:
      if (dir == 0)
        bitClear(shifter_1, 4);
      if (dir == 1)
        bitSet(shifter_1, 4);
      break;
    case MotorZ:
      if (dir == 0)
        bitClear(shifter_0, 0);
      if (dir == 1)
        bitSet(shifter_0, 0);
      break;
    case MotorR:
      if (dir == 0)
        bitClear(shifter_0, 4);
      if (dir == 1)
        bitSet(shifter_0, 4);
      break;
  }
  Load(shifter_0, shifter_1);
}

void Shifter::ChangeSpeed(unsigned char shifter_0, unsigned char shifter_1, int spd, SerialCommunication serialCommunication) {
  switch (spd) {
    case 1:
      bitClear(shifter_0, 2);
      bitClear(shifter_0, 3);
      bitClear(shifter_0, 6);
      bitClear(shifter_0, 7);
      bitClear(shifter_1, 2);
      bitClear(shifter_1, 3);
      bitClear(shifter_1, 6);
      bitClear(shifter_1, 7);
      break;

    case 2:
      bitSet(shifter_0, 2);
      bitClear(shifter_0, 3);
      bitSet(shifter_0, 6);
      bitClear(shifter_0, 7);
      bitSet(shifter_1, 2);
      bitClear(shifter_1, 3);
      bitSet(shifter_1, 6);
      bitClear(shifter_1, 7);
      serialCommunication.Send("SPD2");
      break;

    case 4:
      bitClear(shifter_0, 2);
      bitSet(shifter_0, 3);
      bitClear(shifter_0, 6);
      bitSet(shifter_0, 7);
      bitClear(shifter_1, 2);
      bitSet(shifter_1, 3);
      bitClear(shifter_1, 6);
      bitSet(shifter_1, 7);
      serialCommunication.Send("SPD4");
      break;

    case 8:
      bitSet(shifter_0, 2);
      bitSet(shifter_0, 3);
      bitSet(shifter_0, 6);
      bitSet(shifter_0, 7);
      bitSet(shifter_1, 2);
      bitSet(shifter_1, 3);
      bitSet(shifter_1, 6);
      bitSet(shifter_1, 7);
      serialCommunication.Send("SPD8");
      break;
  }
  Load(shifter_0, shifter_1);
}