#define DIR 10
#define STEP 11
#define MR1 12
#define MR2 13
#define LED A0
#define ULN A1
#define ENDSTOP_X_PLUS 6
#define ENDSTOP_X_MINUS 7
#define ENDSTOP_Y_PLUS 4
#define ENDSTOP_Y_MINUS 5
#define ENDSTOP_Z_PLUS 9
#define ENDSTOP_F_PLUS 2
#define ENDSTOP_F_MINUS 3
#define RX A3
#define TX A2

#include <SoftwareSerial.h>
#include "myStepper.h"

SoftwareSerial mySerial(RX, TX);

char message[4]  = " ";
char endstopXPlus;
char endstopXMinus;
char endstopYPlus;
char endstopYMinus;
char endstopZPlus;
char endstopFPlus;
char endstopFMinus;
char stopped = 0;
char flagX = 0;
char flagx = 0;
char flagY = 0;
char flagy = 0;
char flagZ = 0;
char flagM5 = 0;
char period = 0;

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(MR1, OUTPUT);
  pinMode(MR2, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(ULN, OUTPUT);
  pinMode(ENDSTOP_X_PLUS, INPUT_PULLUP);
  pinMode(ENDSTOP_X_MINUS, INPUT_PULLUP);
  pinMode(ENDSTOP_Y_PLUS, INPUT_PULLUP);
  pinMode(ENDSTOP_Y_MINUS, INPUT_PULLUP);
  pinMode(ENDSTOP_Z_PLUS, INPUT_PULLUP);
  pinMode(ENDSTOP_F_PLUS, INPUT_PULLUP);
  pinMode(ENDSTOP_F_MINUS, INPUT_PULLUP);
  digitalWrite(DIR, 0);
  digitalWrite(STEP, 0);
  digitalWrite(MR1, 1);
  digitalWrite(MR2, 1);
  digitalWrite(LED, 0);
  digitalWrite(ULN, 0);
}

void led(int period) {
  digitalWrite(LED, 1);
  delay(period);
  digitalWrite(LED, 0);
  delay(period);
//  Serial.println(period);
}

void checkEndstops() {
  endstopXPlus = digitalRead(ENDSTOP_X_PLUS);
  endstopXMinus = digitalRead(ENDSTOP_X_MINUS);
  endstopYPlus = digitalRead(ENDSTOP_Y_PLUS);
  endstopYMinus = digitalRead(ENDSTOP_Y_MINUS);
  endstopZPlus = digitalRead(ENDSTOP_Z_PLUS);
  
  if (endstopXPlus == 0 && flagX == 0) {
    flagX = 1;
    delay(100);
    mySerial.write("ENDX");
  }
  if (endstopXPlus == 1) {
    flagX = 0;
  }

  if (endstopXMinus == 0 && flagx == 0) {
    flagx = 1;
    delay(100);
    mySerial.write("ENDx");
  }
  if (endstopXMinus == 1) {
    flagx = 0;
  } 

  if (endstopYPlus == 0 && flagY == 0) {
    flagY = 1;
    delay(100);
    mySerial.write("ENDY");
  }
  if (endstopYPlus == 1) {
    flagY = 0;
  } 

  if (endstopYMinus == 0 && flagy == 0) {
    flagy = 1;
    delay(100);
    mySerial.write("ENDy");
  }
  if (endstopYMinus == 1) {
    flagy = 0;
  } 

  if (endstopZPlus == 0 && flagZ == 0) {
    flagZ = 1;
    delay(100);
    mySerial.write("ENDZ");
  }
  if (endstopZPlus == 1) {
    flagZ = 0;
  } 
}
 
void loop()
{
  led(period);
  checkEndstops();
  if (mySerial.available()) {
    Serial.println(message);
    mySerial.readBytes(message, 4);
    if(strcmp(message, "RLON") == 0) {
      digitalWrite(ULN, 1);
    }
    if(strcmp(message, "RLOF") == 0) {
      digitalWrite(ULN, 0);
    }
    if(strcmp(message, "CHCK") == 0 ||
        strcmp(message, "HCKC") == 0 ||
        strcmp(message, "CKCH") == 0 ||
        strcmp(message, "KCHC") == 0) {
      checkEndstops();
    }
    if(strcmp(message, "SPD2") == 0) {
//      digitalWrite(LED, 0);
      Serial.println("SPD2");
//      delay(50);
      period = 100;
    }
    if(strcmp(message, "SPD4") == 0) {
//      digitalWrite(LED, 0);
      Serial.println("SPD4");
//      delay(50);
      period = 50;
    }
    if(strcmp(message, "SPD8") == 0) {
//      digitalWrite(LED, 0);
      Serial.println("SPD8");
//      delay(50);
      period = 10;
    }
    if(strcmp(message, "M5CL") == 0 ||
        strcmp(message, "5CLM") == 0 ||
        strcmp(message, "CLM5") == 0 ||
        strcmp(message, "LM5C") == 0) {
      endstopFPlus = digitalRead(ENDSTOP_F_PLUS);
      Serial.println("Moving backwards...");
      if (endstopFPlus == 1) {
        stepperMove(1, 5);
      }
      else {
        mySerial.write("END5");
        stepperMove(0,100);
      }
    }
    if(strcmp(message, "M5CC") == 0 ||
        strcmp(message, "CM5C") == 0 ||
        strcmp(message, "CCM5") == 0 ||
        strcmp(message, "5CCM") == 0) {
      endstopFMinus = digitalRead(ENDSTOP_F_MINUS);
      Serial.println("Moving...");
      if (endstopFMinus == 1) {
        stepperMove(0, 5);
      }
      else {
        mySerial.write("END5");
        stepperMove(1,100);
      }
    }
  }
}
