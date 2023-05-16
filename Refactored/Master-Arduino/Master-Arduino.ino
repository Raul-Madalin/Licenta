#include "Position.h"
#include "SerialCommunication.h"
#include "ControlBoard.h"
#include "Shifter.h"
#include "Motor.h"
#include <Arduino.h>

const int SER = 13;
const int SRCLK = A0;
const int RCLK = A1;
const int RX = A2;
const int TX = A3;
const int STEPS = 1850;

Shifter shifter(SER, SRCLK, RCLK);
SerialCommunication serialCommunication(RX, TX);
Motor motorX(MotorX);
Motor motorY(MotorY);
Motor motorZ(MotorZ);
Motor motorR(MotorR);
Motor motorF(MotorF);
Position currentPosition;

int steps = 1600;
int maxSteps = 0;
unsigned char shifter_0 = 0;
unsigned char shifter_1 = 0;
String message;

void origin() {
  shifter.ChangeSpeed(shifter_0, shifter_1, 4, serialCommunication);
  motorZ.ToOrigin(shifter_0, shifter_1, shifter, currentPosition, serialCommunication);
  steps = STEPS;
  maxSteps = 0;
  motorX.ToOrigin(shifter_0, shifter_1, shifter, currentPosition, serialCommunication);
  motorY.ToOrigin(shifter_0, shifter_1, shifter, currentPosition, serialCommunication);

  // TODO: implement rotation origin
  motorR.RotationOrigin();
}

void setup() {
}

void loop() {
}
