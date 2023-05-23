const int SER = 13;
const int SRCLK = A0;
const int RCLK = A1;
const int MAXSTEPS = 7500;
const int ENDSTOP_R_PLUS = 2;
const int ENDSTOP_R_MINUS = 3;
const int RX = A2;
const int TX = A3;
const int CLOCKWISE = 0;
const int COUNTERCLOCKWISE = 1;

#include <SoftwareSerial.h>
#include "myShifter.h"
#include "myStepper.h"
#include "myButtons.h"

SoftwareSerial mySerial(RX, TX);

unsigned char button = ' ';
unsigned char state = 0;
unsigned char hold = 1;
unsigned char pressed = 0;
unsigned char spd = 4;
char endstopRPlus;
char endstopRMinus;
int maxSteps = 0;
char stepsFlag = 1;
char stopFlag = 0;

unsigned char shifter_0 = 0;
unsigned char shifter_1 = 0;
unsigned char buttons[16] = " ";
boolean shifter[16];
String message;

int prevX, prevY, prevZ = -100;
int lastMovement = 0;

struct Position {
  int X, Y, Z, R;
} currentPosition;

enum ButtonsLabels {
  LeftY,
  LeftX,
  UpZ,
  RightR,
  LeftF,
  RightY,
  RightX,
  DownZ,
  LeftR,
  RightF,
  Home,
  MaxZ,
  Speed8,
  Speed4,
  Speed2
};

enum Motors {
  Blank,
  MotorX,
  MotorY,
  MotorZ,
  MotorR,
  MotorF
};

void resetPosition() {
  currentPosition.X = 0;
  currentPosition.Y = 0;
  currentPosition.Z = 0;
}

void reverseDirection(int& direction) {
  if (direction == CLOCKWISE) {
    direction = COUNTERCLOCKWISE;
  } else if (direction == COUNTERCLOCKWISE) {
    direction = CLOCKWISE;
  }
}

void moveToOrigin(Motors motor, String signal, int steps) {
  int direction = CLOCKWISE;
  if (motor == MotorY) {
    reverseDirection(direction);
  }
  Serial.println(motor);
  do {
    mySerial.write("CHCK", 4);
    if (mySerial.available()) {
      message = mySerial.readString();
      message.trim();
    }
    stepperMove(motor, 0, 10);
  } while (message.equals(signal) == false);

  reverseDirection(direction);
  stepperMove(motor, 1, steps);

  if (motor == MotorZ) {
    maxSteps = MAXSTEPS;
  }

  delay(200);
}

void origin() {
  changeSpeed(4);

  // moveToOrigin(MotorZ, "Clockwise", "ENDZ", 150);
  moveToOrigin(MotorX, "ENDx", 100);

  // do {
  //   mySerial.write("CHCK", 4);
  //   if (mySerial.available()) {
  //     message = mySerial.readString();
  //     message.trim();
  //   }
  //   stepperMove(3, 0, 10);
  // } while (message.equals("ENDZ") == false);
  // stepperMove(3, 1, 150);
  // maxSteps = MAXSTEPS;
  // currentPosition.Z = 0;
  // delay(200);

  // do {
  //   mySerial.write("CHCK", 4);
  //   if (mySerial.available()) {
  //     message = mySerial.readString();
  //     message.trim();
  //     Serial.println(message);
  //   }
  //   stepperMove(1, 0, 10);
  // } while (message.equals("ENDx") == false);
  // stepperMove(1, 1, 100);
  // currentPosition.X = 0;
  // delay(200);

  // do {
  //   mySerial.write("CHCK", 4);
  //   if (mySerial.available()) {
  //     message = mySerial.readString();
  //     message.trim();
  //   }
  //   stepperMove(2, 1, 10);
  // } while (message.equals("ENDy") == false);
  // stepperMove(2, 0, 100);  // TODO: more steps
  // currentPosition.Y = 0;
  // delay(200);

  //  changeSpeed(8);
  // do {
  //   delayMicroseconds(5000);
  //   endstopRPlus = digitalRead(ENDSTOP_R_PLUS);
  //   stepperMove(4, 1, 1);
  // } while(endstopRPlus == 1);
  // stepperMove(4, 0, 100);
  // R = 0;
  // delay(200);

  //  do {
  //    mySerial.write("CHCK", 4);
  //    if (mySerial.available()) {
  //      message = mySerial.readString();
  //      message.trim();
  //    }
  //    mySerial.write("M5CC", 4);
  //    Serial.println("MOVING...");
  //  } while(message.equals("END5") == false);
  //  changeSpeed(4);
  //  delay(200);
}

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(RCLK, OUTPUT);
  pinMode(SER, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(ENDSTOP_R_PLUS, INPUT_PULLUP);
  pinMode(ENDSTOP_R_MINUS, INPUT_PULLUP);
  digitalWrite(RCLK, HIGH);
  digitalWrite(SER, 0);
  digitalWrite(SRCLK, HIGH);

  delay(2000);

  Serial.println("START...");
  mySerial.write("RLON", 4);
  delay(100);
  shifterReset();
  origin();
}

void checkEndstops() {
  if (prevX != currentPosition.X || prevY != currentPosition.Y || prevZ != currentPosition.Z) {
    Serial.print("X: ");
    Serial.print(currentPosition.X);
    Serial.print(" | ");

    Serial.print("Y: ");
    Serial.print(currentPosition.Y);
    Serial.print(" | ");

    Serial.print("Z: ");
    Serial.println(currentPosition.Z);

    prevX = currentPosition.X;
    prevY = currentPosition.Y;
    prevZ = currentPosition.Z;
  }

  mySerial.write("CHCK", 4);
  if (mySerial.available()) {
    message = mySerial.readString();
    message.trim();
    Serial.println(message);

    if (message.equals("ENDy") and lastMovement == 1) {
      stepperMove(2, 0, 100);
      currentPosition.Y = 0;
    }
    if (message.equals("ENDY") and lastMovement == 1) {
      stepperMove(2, 1, 100);
      currentPosition.Y = 5600;
    }

    if (message.equals("ENDx") and lastMovement == 2) {
      stepperMove(1, 1, 100);
      currentPosition.X = 0;
    }
    if (message.equals("ENDX") and lastMovement == 2) {
      stepperMove(1, 0, 100);
      currentPosition.X = 4400;
    }

    if (message.equals("ENDZ") and lastMovement == 3) {
      stepperMove(3, 1, 150);
      currentPosition.Z = 0;
    }
  }
}

void loop() {
  checkButtons();
  checkEndstops();
  for (int count = 0; count < 16; count++) {
    if (buttons[count] == 1) {
      switch (count) {

        case RightY:
          lastMovement = 1;
          if (hold == 2) {
            changeSpeed(spd / hold);
          }
          stepperMove(2, 1, 10);
          currentPosition.Y -= 10 / spd * 10;
          break;

        case RightX:
          lastMovement = 2;
          if (hold == 2) {
            changeSpeed(spd / hold);
          }
          stepperMove(1, 1, 10);
          currentPosition.X += 10 / spd * 10;
          break;

        case UpZ:
          lastMovement = 3;
          if (hold == 2 && spd > 2) {
            changeSpeed(spd / hold);
            stepsFlag = 2;
          }
          stepperMove(3, 0, 10);
          currentPosition.Z -= 10 / spd * 10;
          break;

        case RightR:
          changeSpeed(8);
          delayMicroseconds(5000);
          endstopRMinus = digitalRead(ENDSTOP_R_MINUS);
          if (endstopRMinus == 0) {
            stepperMove(4, 1, 30);
            break;
          }
          stepperMove(4, 0, 1);
          break;

        case LeftF:
          mySerial.write("M5CC", 4);
          break;

        case LeftY:
          lastMovement = 1;
          if (hold == 2) {
            changeSpeed(spd / hold);
          }
          stepperMove(2, 0, 10);
          currentPosition.Y += 10 / spd * 10;
          break;

        case LeftX:
          lastMovement = 2;
          if (hold == 2) {
            changeSpeed(spd / hold);
          }
          stepperMove(1, 0, 10);
          currentPosition.X -= 10 / spd * 10;
          break;

        case DownZ:
          lastMovement = 3;
          if (hold == 2 && spd > 2) {
            changeSpeed(spd / hold);
            stepsFlag = 2;
          }
          if (currentPosition.Z < maxSteps) {
            stepperMove(3, 1, 10);
            currentPosition.Z += 10 / spd * 10;
          }
          break;

        case LeftR:
          changeSpeed(8);
          delayMicroseconds(5000);
          endstopRPlus = digitalRead(ENDSTOP_R_PLUS);
          if (endstopRPlus == 0) {
            stepperMove(4, 0, 20);
            break;
          }
          stepperMove(4, 1, 1);
          break;

        case RightF:
          mySerial.write("M5CL", 4);
          Serial.println("MOVING...");
          break;

        case Home:
          if (pressed == 1) {
            origin();
          }
          break;

        case MaxZ:
          Serial.print("[STATUS] Setting max depth... ");
          Serial.println(currentPosition.Z);
          maxSteps = currentPosition.Z;
          break;

        case Speed8:
          spd = 8;
          changeSpeed(spd);
          mySerial.write("SPD8", 4);
          break;

        case Speed4:
          spd = 4;
          changeSpeed(spd);
          mySerial.write("SPD4", 4);
          break;

        case Speed2:
          spd = 2;
          changeSpeed(spd);
          mySerial.write("SPD2", 4);
          break;
      }
    }
  }
  hold = 1;
  pressed = 0;
  button = ' ';
}
