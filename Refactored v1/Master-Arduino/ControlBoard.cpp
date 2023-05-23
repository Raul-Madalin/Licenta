#include "ControlBoard.h"
#include <Arduino.h>
#include <Keypad.h>

ControlBoard::ControlBoard()
  : kpd(makeKeymap(keys), rowPins, colPins, ROWS, COLS) {
  this->ROWS = 3;
  this->COLS = 5;

  this->keys[0][0] = 'a';
  this->keys[0][1] = 'b';
  this->keys[0][2] = 'c';
  this->keys[0][3] = 'd';
  this->keys[0][4] = 'e';
  this->keys[1][0] = 'f';
  this->keys[1][1] = 'g';
  this->keys[1][2] = 'h';
  this->keys[1][3] = 'i';
  this->keys[1][4] = 'j';
  this->keys[2][0] = 'k';
  this->keys[2][1] = 'l';
  this->keys[2][2] = 'm';
  this->keys[2][3] = 'n';
  this->keys[2][4] = 'o';

  this->rowPins[0] = 4;
  this->rowPins[1] = 5;
  this->rowPins[2] = 6;

  this->colPins[0] = 8;
  this->colPins[1] = 9;
  this->colPins[2] = 10;
  this->colPins[3] = 11;
  this->colPins[4] = 12;
}

void ControlBoard::CheckButtons(unsigned char button, unsigned char state, unsigned char buttons[], unsigned char pressed, int stepsFlag, unsigned char hold, unsigned char spd, unsigned char shifter_0, unsigned char shifter_1, Shifter shifter, SoftwareSerial mySerial) {
  if (this->kpd.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {  // Scan the whole key list.
      if (this->kpd.key[i].stateChanged) {      // Only find keys that have changed state.
        switch (this->kpd.key[i].kstate) {      // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            state = 1;  // PRESSED
            break;

          case HOLD:
            state = 2;  // HOLD
            break;

          case RELEASED:
            state = 3;  // RELEASED
            break;

          case IDLE:
            state = 0;  // IDLE
        }

        switch (state) {
          case 0:
            button = kpd.key[i].kchar;
            buttons[button - 97] = 0;
            break;

          case 1:
            button = kpd.key[i].kchar;
            buttons[button - 97] = 1;
            pressed = 1;
            break;

          case 2:
            button = kpd.key[i].kchar;
            buttons[button - 97] = 1;
            hold = 2;
            break;

          case 3:
            button = kpd.key[i].kchar;
            buttons[button - 97] = 1;
            // shifter.ChangeSpeed(shifter_0, shifter_1, spd, serialCommunication);
            stepsFlag = 1;
            break;
        }
      }
    }
  }
}