#ifndef ControlBoard_h
#define ControlBoard_h

#include <Arduino.h>
#include <Keypad.h>
// #include "SerialCommunication.h"
#include "Shifter.h"

class ControlBoard {
private:
  byte ROWS;
  byte COLS;
  char keys[3][5];
  byte rowPins[5];
  byte colPins[5];
  Keypad kpd;

public:
  ControlBoard();
  void CheckButtons(unsigned char button, unsigned char state, unsigned char buttons[], unsigned char pressed, int stepsFlag, unsigned char hold, unsigned char spd, unsigned char shifter_0, unsigned char shifter_1, Shifter shifter, SoftwareSerial mySerial);
};

#endif