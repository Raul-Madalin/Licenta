#include "joystick.h"

#define VRx A0
#define VRy A1
#define SW 2

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;

int mapX = 0;
int mapY = 0;

int state = 0;
int lastState = 0;

void setup() {
  Serial.begin(9600);
  joystickSetup(VRx, VRy, SW);
}

void loop() {

  joystickInput(VRx, VRy, SW);

  if (state == 0) {
    if (SW_state == 1) {
      lastState = state;
      state = 4;
    }
  }

  if (state == 1) {
    joystickMapping(1024);
    
    if (SW_state == 1) {
      lastState = state;
      state = 4;
    }
  }

  if (state == 2) {
    joystickMapping(2048);
    
    if (SW_state == 1) {
      lastState = state;
      state = 4;
    }
  }

  if (state == 3) {
    joystickMapping(16);
    
    if (SW_state == 1) {
      lastState = state;
      state = 4;
    }
  }

  if (state == 4) {
    if (SW_state == 0)
      state = lastState + 1;
  }
  
  Serial.print("X: ");
  Serial.print(mapX);
  Serial.print(" | Y: ");
  Serial.print(mapY);
  Serial.print(" | Button: ");
  Serial.print(SW_state);
  Serial.print(" | State:");
  Serial.println(state);

  delay(100);

}
