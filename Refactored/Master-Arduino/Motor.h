#ifndef Motor_h
#define Motor_h

#include <Arduino.h>
#include "Shifter.h"
#include "Position.h"
#include "SerialCommunication.h"

class Motor {
private:
  Motors name;
public:
  Motor(Motors motorName);
  void Move(unsigned char shifter_0, unsigned char shifter_1, int dir, int stepsPerRevolution, Shifter shifter);
  void ToOrigin(unsigned char shifter_0, unsigned char shifter_1, Shifter shifter, Position currentPosition, SerialCommunication serialCommunication);
  void RotationOrigin();
};

#endif