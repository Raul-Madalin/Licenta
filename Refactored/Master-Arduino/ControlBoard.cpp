#include "ControlBoard.h"
#include <Arduino.h>
#include <Keypad.h>

ControlBoard::ControlBoard()
  : kpd(makeKeymap(keys), rowPins, colPins, ROWS, COLS) {
  ROWS = 3;
  COLS = 5;

  keys[0][0] = 'a';
  keys[0][1] = 'b';
  keys[0][2] = 'c';
  keys[0][3] = 'd';
  keys[0][4] = 'e';
  keys[1][0] = 'f';
  keys[1][1] = 'g';
  keys[1][2] = 'h';
  keys[1][3] = 'i';
  keys[1][4] = 'j';
  keys[2][0] = 'k';
  keys[2][1] = 'l';
  keys[2][2] = 'm';
  keys[2][3] = 'n';
  keys[2][4] = 'o';

  rowPins[0] = 4;
  rowPins[1] = 5;
  rowPins[2] = 6;

  colPins[0] = 8;
  colPins[1] = 9;
  colPins[2] = 10;
  colPins[3] = 11;
  colPins[4] = 12;
}

void ControlBoard::CheckButtons(unsigned char button, unsigned char state, unsigned char buttons[], unsigned char pressed, int stepsFlag, unsigned char hold, unsigned char spd, unsigned char shifter_0, unsigned char shifter_1, Shifter shifter, SerialCommunication serialCommunication) {
  if (kpd.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {  // Scan the whole key list.
      if (kpd.key[i].stateChanged) {      // Only find keys that have changed state.
        switch (kpd.key[i].kstate) {      // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
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
            shifter.ChangeSpeed(shifter_0, shifter_1, spd, serialCommunication);
            stepsFlag = 1;
            break;
        }
      }
    }
  }
}