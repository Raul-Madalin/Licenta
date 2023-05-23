#include "Position.h"
// #include "SerialCommunication.h"
#include "ControlBoard.h"
#include "Shifter.h"
#include "Motor.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

const int SER = 13;
const int SRCLK = A0;
const int RCLK = A1;
const int RX = A2;
const int TX = A3;
const int STEPS = 1850;
const int ENDSTOP_R_PLUS = 2;
const int ENDSTOP_R_MINUS = 3;

Shifter shifter(SER, SRCLK, RCLK);
// SerialCommunication serialCommunication(RX, TX);
SoftwareSerial mySerial(RX, TX);
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
  shifter.ChangeSpeed(shifter_0, shifter_1, 4, mySerial);
  do {
    mySerial.write("CHCK", 4);
    if (mySerial.available()) {
      message = mySerial.readString();
      message.trim();
    }
    motorZ.Move(shifter_0, shifter_1, 0, 10, shifter);
  } while (message.equals("ENDZ") == false);
  motorZ.Move(shifter_0, shifter_1, 1, 100, shifter);
  steps = STEPS;
  maxSteps = 0;
  // Z = 0;
  delay(200);

  do {
    mySerial.write("CHCK", 4);
    if (mySerial.available()) {
      message = mySerial.readString();
      message.trim();
      Serial.println(message);
    }
    motorX.Move(shifter_0, shifter_1, 0, 10, shifter);
  } while (message.equals("ENDx") == false);
  motorX.Move(shifter_0, shifter_1, 1, 100, shifter);
  // X = 0;
  delay(200);

  do {
    mySerial.write("CHCK", 4);
    if (mySerial.available()) {
      message = mySerial.readString();
      message.trim();
    }
    motorY.Move(shifter_0, shifter_1, 1, 10, shifter);
  } while (message.equals("ENDy") == false);
  motorZ.Move(shifter_0, shifter_1, 0, 100, shifter);
  // stepperMove(2, 0, 100);  // TODO: more steps
  // Y = 0;
  delay(200);

  //  changeSpeed(8);
  // do {
  //   delayMicroseconds(5000);
  //   endstopRPlus = digitalRead(ENDSTOP_R_PLUS);
  //   stepperMove(4, 1, 1);
  // } while(endstopRPlus == 1);
  // stepperMove(4, 0, 100);
  // R = 0;
  // delay(200);
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(ENDSTOP_R_PLUS, INPUT_PULLUP);
  pinMode(ENDSTOP_R_MINUS, INPUT_PULLUP);

  delay(2000);
  Serial.println("[STATUS] START...");
  mySerial.write("RLON", 4);

  delay(100);
  shifter.Reset(shifter_0, shifter_1);
  origin();
}

void loop() {
}
