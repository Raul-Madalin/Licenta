/* @file MultiKey.ino
  || @version 1.0
  || @author Mark Stanley
  || @contact mstanley@technologist.com
  ||
  || @description
  || | The latest version, 3.0, of the keypad library supports up to 10
  || | active keys all being pressed at the same time. This sketch is an
  || | example of how you can get multiple key presses from a keypad or
  || | keyboard.
  || #
*/

#include <Keypad.h>
#include "myHeader.h"

const byte ROWS = 3; //four rows
const byte COLS = 5; //three columns
char keys[ROWS][COLS] = {
  {'X', 'Y', 'Z', 'R', 'F'},
  {'x', 'y', 'z', 'r', 'f'},
  {'H', 'S', '1', '2', '3'}
};
byte rowPins[ROWS] = {4, 5, 6}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {8, 9, 10, 11, 12}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

unsigned long loopCount;
unsigned long startTime;
String msg;

void test(String message) {
  if (message == " HOLD.")
    Serial.println("TEST");
}


void setup() {
  Serial.begin(9600);
  loopCount = 0;
  startTime = millis();
  msg = "";
}


void loop() {
  loopCount++;
  if ( (millis() - startTime) > 5000 ) {
    startTime = millis();
    loopCount = 0;
  }

  // Fills kpd.key[ ] array with up-to 10 active keys.
  // Returns true if there are ANY active keys.
  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++) // Scan the whole key list.
    {
      if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
      {
        switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            msg = " PRESSED.";
            break;
          case HOLD:
            msg = " HOLD.";
            break;
          case RELEASED:
            msg = " RELEASED.";
            break;
          case IDLE:
            msg = " IDLE.";
        }
        Serial.print("Key ");
        Serial.print(kpd.key[i].kchar);
        Serial.println(msg);
        test(msg);
      }
    }
  }
}  // End loop
