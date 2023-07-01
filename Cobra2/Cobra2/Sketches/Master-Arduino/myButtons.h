#include <Keypad.h>
const byte ROWS = 3; //four rows
const byte COLS = 5; //three columns
//char keys[ROWS][COLS] = {
//  {'X', 'Y', 'Z', 'R', 'F'},
//  {'x', 'y', 'z', 'r', 'f'},
//  {'H', 'S', '1', '2', '3'}
//};
char keys[ROWS][COLS] = {
  {'a', 'b', 'c', 'd', 'e'},
  {'f', 'g', 'h', 'i', 'j'},
  {'k', 'l', 'm', 'n', 'o'}
};
byte rowPins[ROWS] = {4, 5, 6}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {8, 9, 10, 11, 12}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

extern unsigned char state;
extern unsigned char button;
extern unsigned char buttons[16];
extern unsigned char hold;
extern unsigned char spd;
extern unsigned char pressed;
extern char stepsFlag;
extern int steps;
extern char stopFlag;
extern int maxSteps;

void checkButtons() {
  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++) {  // Scan the whole key list.
      if ( kpd.key[i].stateChanged ) {  // Only find keys that have changed state.
        switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            state = 1;  // PRESSED
            break;
          case HOLD:
            state = 2; // HOLD
            break;
          case RELEASED:
            state = 3; // RELEASED
            break;
          case IDLE:
            state = 0; // IDLE
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
            changeSpeed(spd);
            stepsFlag = 1;
            break;
        }
      }
    }
  }
}
