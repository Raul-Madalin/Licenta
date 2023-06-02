#ifndef Shifter_h
#define Shifter_h

#include <Arduino.h>
// #include "SerialCommunication.h"
#include <SoftwareSerial.h>

enum Motors {
  MotorX,
  MotorY,
  MotorZ,
  MotorR,
  MotorF,
};

class Shifter {
private:
  int SER;
  int SRCLK;
  int RCLK;

public:
  Shifter(int serPin, int srclkPin, int rclkPin);
  void Load(unsigned char shifter_0, unsigned char shifter_1);
  void Reset(unsigned char& shifter_0, unsigned char& shifter_1);
  void Mapping(unsigned char shifter_0, unsigned char shifter_1, Motors motor, int dir);
  void ChangeSpeed(unsigned char shifter_0, unsigned char shifter_1, int spd, SoftwareSerial mySerial);
};

#endif