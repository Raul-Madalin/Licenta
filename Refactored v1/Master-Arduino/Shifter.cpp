#include "Shifter.h"
#include <Arduino.h>

Shifter::Shifter(int serPin, int srclkPin, int rclkPin) {
  this->SER = serPin;
  this->SRCLK = srclkPin;
  this->RCLK = rclkPin;

  pinMode(this->RCLK, OUTPUT);
  pinMode(this->SER, OUTPUT);
  pinMode(this->SRCLK, OUTPUT);

  digitalWrite(this->RCLK, HIGH);
  digitalWrite(this->SER, 0);
  digitalWrite(this->SRCLK, HIGH);  
}

void Shifter::Load(unsigned char shifter_0, unsigned char shifter_1) {
  digitalWrite(this->RCLK, LOW);
  shiftOut(this->SER, this->SRCLK, MSBFIRST, shifter_0);
  shiftOut(this->SER, this->SRCLK, MSBFIRST, shifter_1);
  digitalWrite(this->RCLK, HIGH);
}

void Shifter::Reset(unsigned char& shifter_0, unsigned char& shifter_1) {
  shifter_0 = 0;
  shifter_1 = 0;
  Load(shifter_0, shifter_1);
}

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

void Shifter::ChangeSpeed(unsigned char shifter_0, unsigned char shifter_1, int spd, SoftwareSerial mySerial) {
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
      mySerial.write("SPD2");
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
      mySerial.write("SPD4");
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
      mySerial.write("SPD8");
      break;
  }
  Load(shifter_0, shifter_1);
}