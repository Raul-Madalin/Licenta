const int SER = 13;
const int SRCLK = A0;
const int RCLK = A1;
const int MAXSTEPS = 10000;
const int ENDSTOP_R_PLUS = 2;
const int ENDSTOP_R_MINUS = 3;
const int RX = A2;
const int TX = A3;
const int CLOCKWISE = 0;
const int COUNTERCLOCKWISE = 1;
const int SMALLSTEPS = 1;
const int BIGSTEPS = 10;
const int ROTATIONBACKSTEPS = 30;
const int SMALLBACKSTEPS = 100;
const int BIGBACKSTEPS = 150;
const int RESETR = 560;
const int MILISECONDS = 200;
const int MICROSECONDS = 5000;
const int ENDXPOSITION = 8300;
const int ENDYPOSITION = 14000;
const int EEADDR = 0;

#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "myShifter.h"
#include "myStepper.h"
#include "myButtons.h"
#include "myPosition.h"

SoftwareSerial mySerial(RX, TX);

unsigned char button = ' ';
unsigned char state = 0;
unsigned char hold = 1;
unsigned char targetComponentActive = 0;
unsigned char pressed = 0;
unsigned char spd = 4;
char endstopRPlus;
char endstopRMinus;
int maxSteps = 0;
char stepsFlag = 1;
char stopFlag = 0;
int EEAddr = 0;
double newZ = 0.;
int tarCompX = 0;

unsigned char shifter_0 = 0;
unsigned char shifter_1 = 0;
unsigned char buttons[16] = " ";
boolean shifter[16];
String message;

int prevX, prevY, prevZ = -100;
float prevR = -100;

Steppers lastMovement = Blank;

// TODO: implement focus movement
void origin() {
  changeSpeed(4);

  moveToOrigin(StepperZ, "ENDZ", BIGSTEPS, BIGBACKSTEPS, maxSteps);
  moveToOrigin(StepperX, "ENDx", BIGSTEPS, SMALLBACKSTEPS, maxSteps);
  moveToOrigin(StepperY, "ENDy", BIGSTEPS, SMALLBACKSTEPS, maxSteps);
  rotateToOrigin(StepperR, SMALLSTEPS, RESETR);
  resetPosition(currentPosition);
  targetComponentActive = 0;

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
  changeSpeed(4);
}

void checkEndstops() {
  printCurrentPosition(prevX, prevY, prevZ, prevR);

  mySerial.write("CHCK", 4);
  if (mySerial.available()) {
    message = mySerial.readString();
    message.trim();
    Serial.println(message);

    if (message.equals("ENDx") and lastMovement == StepperX) {
      stepperMove(StepperX, CounterClockwise, SMALLBACKSTEPS);
      currentPosition.X = 0;
    }
    if (message.equals("ENDX") and lastMovement == StepperX) {
      stepperMove(StepperX, Clockwise, SMALLBACKSTEPS);
      currentPosition.X = ENDXPOSITION;
    }

    if (message.equals("ENDy") and lastMovement == StepperY) {
      stepperMove(StepperY, Clockwise, SMALLBACKSTEPS);
      currentPosition.Y = 0;
    }
    if (message.equals("ENDY") and lastMovement == StepperY) {
      stepperMove(StepperY, CounterClockwise, SMALLBACKSTEPS);
      currentPosition.Y = ENDYPOSITION;
    }

    if (message.equals("ENDZ") and lastMovement == StepperZ) {
      stepperMove(StepperZ, CounterClockwise, BIGBACKSTEPS);
      currentPosition.Z = 0;
    }
  }
}

void loop() {
  checkButtons();

  for (int count = 0; count < 16; count++) {
    if (buttons[count] == 1) {
      switch (count) {

        case LeftX:   // X+
          checkEndstops();
          lastMovement = StepperX;
          // if (hold == 2) {
          //   changeSpeed(spd / hold);
          // }
          if (targetComponentActive == 1) {
            currentPosition.X -= BIGSTEPS / spd * BIGSTEPS;
            targetComponent.X -= BIGSTEPS / spd * BIGSTEPS;
            
            
            targetComponent.Z = MAXSTEPS - sqrt(targetComponent.radius + targetComponent.X) * sqrt(targetComponent.radius - targetComponent.X);
            // targetComponent.R = asin(double(targetComponent.X) / double(targetComponent.radius)) * 180 / M_PI;
            // targetComponent.R = targetComponent.R * 1.1;

            stepperMove(StepperX, Clockwise, BIGSTEPS);
            moveToPosition(StepperZ, spd, BIGSTEPS, targetComponent.Z, currentPosition.Z);
            // rotateToPosition(StepperR, spd, SMALLSTEPS, targetComponent.R, currentPosition.R);
            changeSpeed(8);
            stepperMove(StepperR, Clockwise, SMALLSTEPS * 2);
            changeSpeed(spd);

            Serial.print("X: ");
            Serial.print(targetComponent.X);
            Serial.print(" | ");

            Serial.print("radius: ");
            Serial.print(targetComponent.radius);
            Serial.print(" | ");

            Serial.print("Z: ");
            Serial.print(targetComponent.Z);
            Serial.print(" | ");

            Serial.print("R: ");
            Serial.println(targetComponent.R);
          }
          else {
            stepperMove(StepperX, Clockwise, BIGSTEPS);
            currentPosition.X -= BIGSTEPS / spd * BIGSTEPS;
          }
          break;

        case RightX:    // X-
          checkEndstops();
          lastMovement = StepperX;
          // if (hold == 2) {
          //   changeSpeed(spd / hold);
          // }
          if (targetComponentActive == 1) {
            currentPosition.X += BIGSTEPS / spd * BIGSTEPS;
            targetComponent.X += BIGSTEPS / spd * BIGSTEPS;
            
            targetComponent.Z = MAXSTEPS - sqrt(targetComponent.radius + targetComponent.X) * sqrt(targetComponent.radius - targetComponent.X);
            // targetComponent.R = asin(double(targetComponent.X) / double(targetComponent.radius)) * 180 / M_PI;
            // targetComponent.R = targetComponent.R * 1.32;

            stepperMove(StepperX, CounterClockwise, BIGSTEPS);
            moveToPosition(StepperZ, spd, BIGSTEPS, targetComponent.Z, currentPosition.Z);
            // rotateToPosition(StepperR, spd, SMALLSTEPS, targetComponent.R, currentPosition.R);
            changeSpeed(8);
            stepperMove(StepperR, CounterClockwise, SMALLSTEPS * 2);
            changeSpeed(spd);

            Serial.print("X: ");
            Serial.print(targetComponent.X);
            Serial.print(" | ");

            Serial.print("radius: ");
            Serial.print(targetComponent.radius);
            Serial.print(" | ");

            Serial.print("Z: ");
            Serial.print(targetComponent.Z);
            Serial.print(" | ");

            Serial.print("R: ");
            Serial.println(targetComponent.R);
          }
          else {
            stepperMove(StepperX, CounterClockwise, BIGSTEPS);
            currentPosition.X += BIGSTEPS / spd * BIGSTEPS;
          }
          break;

        // -------------------------------------------------------------------------------------------

        case LeftY:
          checkEndstops();
          lastMovement = StepperY;
          // if (hold == 2) {
          //   changeSpeed(spd / hold);
          // }
          stepperMove(StepperY, Clockwise, BIGSTEPS);
          currentPosition.Y += BIGSTEPS / spd * BIGSTEPS;
          break;

        case RightY:
          checkEndstops();
          lastMovement = StepperY;
          // if (hold == 2) {
          //   changeSpeed(spd / hold);
          // }
          stepperMove(StepperY, CounterClockwise, BIGSTEPS);
          currentPosition.Y -= BIGSTEPS / spd * BIGSTEPS;
          break;

        // -------------------------------------------------------------------------------------------

        case UpZ:   // Z-
          checkEndstops();
          lastMovement = StepperZ;
          // if (hold == 2) {
          //   changeSpeed(spd / hold);
          // }
          if (targetComponentActive == 1) {
            // Target Component on Z axys not computational possible
            
            // currentPosition.Z -= BIGSTEPS / spd * BIGSTEPS;
            // targetComponent.Z -= BIGSTEPS / spd * BIGSTEPS;
            
            // // targetComponent.R = asin(double(targetComponent.X) / double(sqrt(sq(targetComponent.X) + sq(targetComponent.Z)))) * 180 / M_PI;
            // Serial.println( asin( double(targetComponent.X) / double(targetComponent.X * targetComponent.X + targetComponent.Z * targetComponent.Z)) * 180 / M_PI );
            // targetComponent.radians = targetComponent.R * M_PI / 180;
            // targetComponent.radius = targetComponent.Z / cos(targetComponent.radians);

            // stepperMove(StepperZ, Clockwise, BIGSTEPS);
            // // rotateToPosition(StepperR, spd, SMALLSTEPS, targetComponent.R, currentPosition.R);

            // Serial.print("X: ");
            // Serial.print(targetComponent.X);
            // Serial.print(" | ");

            // Serial.print("radius: ");
            // Serial.print(targetComponent.radius);
            // Serial.print(" | ");

            // Serial.print("Z: ");
            // Serial.print(targetComponent.Z);
            // Serial.print(" | ");

            // Serial.print("R: ");
            // Serial.println(targetComponent.R);
          }
          else {
            stepperMove(StepperZ, Clockwise, BIGSTEPS);
            currentPosition.Z -= BIGSTEPS * BIGSTEPS / (spd * 2);
          }
          break;
        
        case DownZ:   // Z+
          checkEndstops();
          lastMovement = StepperZ;
          // if (hold == 2 && spd > 2) {
          //   changeSpeed(spd / hold);
          //   stepsFlag = 2;
          // }
          if (currentPosition.Z < MAXSTEPS) {
              if (targetComponentActive == 1) {
              // currentPosition.X -= BIGSTEPS / spd * BIGSTEPS;
              // targetComponent.X -= BIGSTEPS / spd * BIGSTEPS;
              
              // targetComponent.Z = MAXSTEPS - sqrt(targetComponent.radius + targetComponent.X) * sqrt(targetComponent.radius - targetComponent.X);
              float xx = float(targetComponent.X) * float(targetComponent.X);
              targetComponent.R = asin(float(targetComponent.X) / sqrt(float(targetComponent.X) * float(targetComponent.X) + float(targetComponent.Z) * float(targetComponent.Z))) * 180 / M_PI;

              // stepperMove(StepperX, CounterClockwise, BIGSTEPS);
              // moveToPosition(StepperZ, spd, BIGSTEPS, targetComponent.Z, currentPosition.Z);
              // rotateToPosition(StepperR, spd, SMALLSTEPS, targetComponent.R, currentPosition.R);

              Serial.print("X: ");
              Serial.print(targetComponent.X);
              Serial.print(" | ");

              Serial.print("radius: ");
              Serial.print(targetComponent.radius);
              Serial.print(" | ");

              Serial.print("Z: ");
              Serial.print(targetComponent.Z);
              Serial.print(" | ");

              Serial.print("R: ");
              Serial.println(targetComponent.R);
            }
            else {
              stepperMove(StepperZ, CounterClockwise, BIGSTEPS);
              currentPosition.Z += BIGSTEPS * BIGSTEPS / (spd * 2);
            }
          }
          break;

        // -------------------------------------------------------------------------------------------

        case LeftR:
          lastMovement = StepperX;
          // if (hold == 2) {
          //   changeSpeed(spd / hold);
          // }
          if (targetComponentActive == 1) {
            currentPosition.R += 0.5 / spd * SMALLSTEPS;
            targetComponent.R += 0.5 / spd * SMALLSTEPS;
            targetComponent.radians = targetComponent.R * M_PI / 180;

            targetComponent.Z = targetComponent.radius * cos(targetComponent.radians);
            targetComponent.X = targetComponent.Z * sin(targetComponent.radians) / cos(targetComponent.radians);
            targetComponent.Z = MAXSTEPS - (targetComponent.radius * cos(targetComponent.radians));

            stepperMove(StepperR, CounterClockwise, SMALLSTEPS);
            moveToPosition(StepperZ, spd, BIGSTEPS, targetComponent.Z, currentPosition.Z);
            moveToPosition(StepperX, spd, BIGSTEPS, currentPosition.X - (tarCompX - targetComponent.X), currentPosition.X);

            Serial.print("X: ");
            Serial.print(targetComponent.X);
            Serial.print(" | ");

            Serial.print("radius: ");
            Serial.print(targetComponent.radius);
            Serial.print(" | ");

            Serial.print("Z: ");
            Serial.print(targetComponent.Z);
            Serial.print(" | ");

            Serial.print("R: ");
            Serial.println(targetComponent.R);
          }
          else {
            changeSpeed(8);
            delayMicroseconds(5000);
            endstopRPlus = digitalRead(ENDSTOP_R_PLUS);
            if (endstopRPlus == 0) {
              stepperMove(StepperR, Clockwise, ROTATIONBACKSTEPS);
              currentPosition.R -= 0.5 / 8 * ROTATIONBACKSTEPS;
              break;
            }
            stepperMove(StepperR, CounterClockwise, SMALLSTEPS);
            currentPosition.R += 0.5 / 8 * SMALLSTEPS;
          }
          break;

        case RightR:
          changeSpeed(8);
          delayMicroseconds(5000);
          endstopRMinus = digitalRead(ENDSTOP_R_MINUS);
          if (endstopRMinus == 0) {
            Serial.println("Click");
            stepperMove(StepperR, CounterClockwise, ROTATIONBACKSTEPS);
            currentPosition.R += 0.5 / 8 * ROTATIONBACKSTEPS;
            break;
          }
          stepperMove(StepperR, Clockwise, SMALLSTEPS);
          currentPosition.R -= 0.5 / 8 * SMALLSTEPS;
          break;

        // -------------------------------------------------------------------------------------------

        case LeftF:
          mySerial.write("M5CC", 4);
          Serial.println("Moving backwards...");
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

        case Speed1:
          if (pressed == 1) {
            switch(spd) {
              // case 2:
              //   spd = 4;
              //   changeSpeed(spd);
              //   mySerial.write("SPD4", 4);
              //   Serial.print("[STATUS] Changing speed... ");
              //   Serial.println(spd);
              //   break;
              case 4:
                spd = 8;
                changeSpeed(spd);
                mySerial.write("SPD8", 4);
                Serial.print("[STATUS] Changing speed... ");
                Serial.println(spd);
                break;
              case 8:
                spd = 4;
                changeSpeed(spd);
                mySerial.write("SPD4", 4);
                Serial.print("[STATUS] Changing speed... ");
                Serial.println(spd);
                break;
            }
          }
          break;

        case Speed2:
          if (pressed == 1) {
            Serial.print("[STATUS] Target position... ");

            if (currentPosition.X < 100 && currentPosition.Y < 100 && currentPosition.Z < 100 && currentPosition.R < 2. && currentPosition.R > -2.) {
              Serial.println("Going to position !");

              EEAddr = EEADDR;
              EEPROM.get(EEAddr, targetPosition.X); EEAddr +=sizeof(targetPosition.X);
              EEPROM.get(EEAddr, targetPosition.Y); EEAddr +=sizeof(targetPosition.Y);
              EEPROM.get(EEAddr, targetPosition.Z); EEAddr +=sizeof(targetPosition.Z);
              EEPROM.get(EEAddr, targetPosition.R); EEAddr +=sizeof(targetPosition.R);

              moveToPosition(StepperX, spd, BIGSTEPS, targetPosition.X, currentPosition.X);
              moveToPosition(StepperY, spd, BIGSTEPS, targetPosition.Y, currentPosition.Y);
              moveToPosition(StepperZ, spd, BIGSTEPS, targetPosition.Z, currentPosition.Z);
              rotateToPosition(StepperR, spd, SMALLSTEPS, targetPosition.R, currentPosition.R);
            }
            else {
              Serial.println("Saving position !");

              EEAddr = EEADDR;
              EEPROM.put(EEAddr, currentPosition.X); EEAddr +=sizeof(currentPosition.X);
              EEPROM.put(EEAddr, currentPosition.Y); EEAddr +=sizeof(currentPosition.Y);
              EEPROM.put(EEAddr, currentPosition.Z); EEAddr +=sizeof(currentPosition.Z);
              EEPROM.put(EEAddr, currentPosition.R); EEAddr +=sizeof(currentPosition.R);
            }
          }

          break;

        case Speed3:
          if (pressed == 1 && targetComponentActive == 0) {
            Serial.println("[STATUS] Target component activated... ");
            targetComponentActive = 1;
            targetComponent.Z = MAXSTEPS - currentPosition.Z;
            targetComponent.R = currentPosition.R;
            targetComponent.radians = targetComponent.R * M_PI / 180;
            targetComponent.radius = targetComponent.Z / cos(targetComponent.radians);
            targetComponent.X = targetComponent.radius * sin(targetComponent.radians);
            tarCompX = targetComponent.X;

            Serial.print("X: ");
            Serial.print(targetComponent.X);
            Serial.print(" | ");

            Serial.print("radius: ");
            Serial.print(targetComponent.radius);
            Serial.print(" | ");

            Serial.print("Z: ");
            Serial.print(targetComponent.Z);
            Serial.print(" | ");

            Serial.print("R: ");
            Serial.println(targetComponent.R);
          }

          else if (pressed == 1 && targetComponentActive == 1) {
          Serial.println("[STATUS] Target component deactivated... ");
            targetComponentActive = 0;
          }
          
          break;
      }
    }
  }
  hold = 1;
  pressed = 0;
  button = ' ';
}
