#include "Motor.h"
#include <Arduino.h>

Motor::Motor(Motors motorName) {
  this->name = motorName;
}

void Motor::Move(unsigned char shifter_0, unsigned char shifter_1, int dir, int stepsPerRevolution, Shifter shifter) {
  Serial.print("[STATUS] Moving: ");
  Serial.println(this->name);
  shifter.Mapping(shifter_0, shifter_1, this->name, dir);
  for (int count = 0; count < stepsPerRevolution; count++) {
    switch (this->name) {
      case MotorX:
        bitSet(shifter_1, 1);
        shifter.Load(shifter_0, shifter_1);
        delayMicroseconds(1000);

        bitClear(shifter_1, 1);
        shifter.Load(shifter_0, shifter_1);
        delayMicroseconds(1000);
        break;

      case MotorY:
        bitSet(shifter_1, 5);
        delayMicroseconds(1000);
        shifter.Load(shifter_0, shifter_1);
        bitClear(shifter_1, 5);
        delayMicroseconds(1000);
        shifter.Load(shifter_0, shifter_1);
        break;

      case MotorZ:
        bitSet(shifter_0, 1);
        delayMicroseconds(1000);
        shifter.Load(shifter_0, shifter_1);
        bitClear(shifter_0, 1);
        delayMicroseconds(1000);
        shifter.Load(shifter_0, shifter_1);
        break;

      case MotorR:
        bitSet(shifter_0, 5);
        delayMicroseconds(1000);
        shifter.Load(shifter_0, shifter_1);
        bitClear(shifter_0, 5);
        delayMicroseconds(1000);
        shifter.Load(shifter_0, shifter_1);
        break;
    }
  }
}

void Motor::ToOrigin(unsigned char shifter_0, unsigned char shifter_1, Shifter shifter, Position currentPosition, SoftwareSerial mySerial) {
  Serial.print("[STATUS] To origin: ");
  Serial.println(this->name);

  String message = "";
  String endStopName = "";

  switch (this->name) {
    case MotorX:
      endStopName = "ENDx";
      break;

    case MotorY:
      endStopName = "ENDy";
      break;

    case MotorZ:
      endStopName = "ENDZ";
      break;
  }

  // do {
    // mySerial.write("CHCK");
    // serialCommunication.Send("CHCK");
    // message = serialCommunication.Receive();
    // if (mySerial.available() > 0) {
      // message = mySerial.readString();
      // message.trim();
      // Serial.println("[STATUS] Received: " + message);
    // }

    // message = mySerial.readString();
    // message.trim();
    // Serial.println("[STATUS] Received: " + message);
    // Move(shifter_0, shifter_1, 0, 10, shifter);

  // } while (message.equals(endStopName) == false);

  // Move(shifter_0, shifter_1, 1, 200, shifter);
  delay(200);
}

void Motor::RotationOrigin() {
  
}