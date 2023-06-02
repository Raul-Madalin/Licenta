#define SER 13
#define SRCLK A0
#define RCLK A1
#define STEPS 1850
#define ENDSTOP_R_PLUS 2
#define ENDSTOP_R_MINUS 3
#define RX A2
#define TX A3

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
int steps = 1600;
int maxSteps = 0;
char stepsFlag = 1;
char stopFlag = 0;

unsigned char shifter_0 = 0;
unsigned char shifter_1 = 0;
unsigned char buttons[16] = " ";
boolean shifter[16];
String message;

int X, Y, Z, R;
int prevY = -100;
int lastMovement = 0;

void origin() {
  changeSpeed(4);
  do {
    mySerial.write("CHCK", 4);
    if (mySerial.available()) {
      message = mySerial.readString();
      message.trim();
    }
    stepperMove(3, 0, 10);
  } while(message.equals("ENDZ") == false);
  stepperMove(3, 1, 200);
  steps = STEPS;
  maxSteps = 0;
  Z = 0;
  delay(200);
  
  do {
    mySerial.write("CHCK", 4);
    if (mySerial.available()) {
      message = mySerial.readString();
      message.trim();
      Serial.println(message);
    }
    stepperMove(1, 0, 10);
  } while(message.equals("ENDx") == false);
  stepperMove(1, 1, 100);
  X = 0;
  delay(200);
  
  do {
    mySerial.write("CHCK", 4);
    if (mySerial.available()) {
      message = mySerial.readString();
      message.trim();
    }
    stepperMove(2, 1, 10);
  } while(message.equals("ENDy") == false);
  stepperMove(2, 0, 100);   // TODO: more steps
  Y = 0;
  delay(200);
  
//  changeSpeed(8);
  do {
    delayMicroseconds(5000);
    endstopRPlus = digitalRead(ENDSTOP_R_PLUS);
    stepperMove(4, 1, 1);
  } while(endstopRPlus == 1);
  stepperMove(4, 0, 100);
  R = 0;
  delay(200);

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

void setup()
{
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

void checkEndstops()
{
//  check endstop functionality
  mySerial.write("CHCK", 4);
  if (mySerial.available()) {
    message = mySerial.readString();
    message.trim();
    if(message.equals("ENDy") and lastMovement == 1) {
      stepperMove(2, 0, 100);
      Y = 0;
    }
    if(message.equals("ENDY") and lastMovement == 1) {
      stepperMove(2, 1, 100);
      Y = 5600;
    }
  }
}

void loop()
{
  if (prevY != Y)
  {
    Serial.println(Y);
    prevY = Y;
  }
  checkButtons();
  checkEndstops();
  for (int count = 0; count < 16; count++) {
    if (buttons[count] == 1) {
      switch (count) {
        case 5:
          // Y axis --> right
          lastMovement = 1;
          if (hold == 2) {
            changeSpeed(spd / 2);
          }
//          mySerial.write("CHCK", 4);
//          if (mySerial.available()) {
//            message = mySerial.readString();
//            message.trim();
//            if(message.equals("ENDy")) {
//              stepperMove(2, 0, 100);
//              Y = 40;
//              break;
//            }
//          }
          stepperMove(2, 1, 10);
          Y -= 10 / spd * 10;
          break;
          
        case 6:
          // X axis --> right
          if (hold == 2) {
            changeSpeed(spd / hold);
          }
          mySerial.write("CHCK", 4);
          if (mySerial.available()) {
            message = mySerial.readString();
            message.trim();
            if(message.equals("ENDX")) {
              stepperMove(1, 0, 100);
              break;
            }
          }
          stepperMove(1, 1, 10);
          // X += 10;
          break;
          
        case 2:
          // Z axis --> up
          if (hold == 2 && spd > 2) {
            changeSpeed(spd / hold);
            stepsFlag = 2;
          }
          mySerial.write("CHCK", 4);
          if (mySerial.available()) {
            message = mySerial.readString();
            message.trim();
            if(message.equals("ENDZ")) {
              stepperMove(3, 1, 100);
              steps = STEPS;
              maxSteps = 0;
              break;
            }
          }
          stepperMove(3, 0, 10);
          steps += (8 / spd) * stepsFlag;
          stopFlag = 0;
          break;
          
        case 3:
          // R axis --> right
          changeSpeed(8);
          delayMicroseconds(5000);
          endstopRMinus = digitalRead(ENDSTOP_R_MINUS);
          if (endstopRMinus == 0) {
            stepperMove(4, 1, 30);
            break;
          }
          stepperMove(4, 0, 1);
          break;
          
        case 4:
          // F axis --> left
          mySerial.write("M5CC", 4);
          break;
          
        case 0:
          // Y axis --> left
          lastMovement = 1;
          if (hold == 2) {
            changeSpeed(spd / hold);
          }
//          mySerial.write("CHCK", 4);
//          if (mySerial.available()) {
//            message = mySerial.readString();
//            message.trim();
//            if(message.equals("ENDY")) {
//              stepperMove(2, 1, 100);
//              Y -= 5500;
//              break;
//            }
//          }
          stepperMove(2, 0, 10);
          Y += 10 / spd * 10;
          break;
          
        case 1:
          // X axis --> left
          if (hold == 2) {
            changeSpeed(spd / hold);
          }
          mySerial.write("CHCK", 4);
          if (mySerial.available()) {
            message = mySerial.readString();
            message.trim();
            if(message.equals("ENDx")) {
              stepperMove(1, 1, 100);
              break;
            }
          }
          stepperMove(1, 0, 10);
          // X -= 10;
          break;
          
        case 7:
          // Z axis --> down
          if (hold == 2 && spd > 2) {
            changeSpeed(spd / hold);
            stepsFlag = 2;
          }
          
          if (steps > 0 || steps > maxSteps) steps -= (8 / spd) * stepsFlag;
          else steps = 0;
          
          if ((steps <= 0 || steps < maxSteps) && stopFlag == 0) {
            stopFlag = 1;
          }
          if (stopFlag == 0) stepperMove(3, 1, 10);
          break;
          
        case 8:
          // R axis --> left
          changeSpeed(8);
          delayMicroseconds(5000);
          endstopRPlus = digitalRead(ENDSTOP_R_PLUS);
          if (endstopRPlus == 0) {
            stepperMove(4, 0, 20);
            break;
          }
          stepperMove(4, 1, 1);
          break;
         
        case 9:
          // F axis --> right
          mySerial.write("M5CL", 4);
          Serial.println("MOVING...");
          break;
          
        case 10:
          if (pressed == 1) {
            origin();
          }
          break;
          
        case 11:
          maxSteps = steps; // TODO: unset maxSteps
          break;
          
        case 12:
          spd = 8;
          changeSpeed(spd);
          mySerial.write("SPD8", 4);
          break;
          
        case 13:
          spd = 4;
          changeSpeed(spd);
          mySerial.write("SPD4", 4);
          break;
          
        case 14:
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
