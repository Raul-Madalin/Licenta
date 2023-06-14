enum Steppers {
  Blank,
  StepperX,
  StepperY,
  StepperZ,
  StepperR,
  StepperF
};

enum Directions {
  Clockwise,
  CounterClockwise
};

void stepperMove(int stepper, Directions dir, int stepsPerRevolution) {
  shifterMapping(stepper, dir);
  //  digitalWrite(LED, HIGH);
  //  stepsPerRevolution = 5; // TODO: leave as parameter
  for (int count = 0; count < stepsPerRevolution; count++) {
    switch (stepper) {
      case 1:
        bitSet(shifter_1, 1);
        shifterLoad();
        delayMicroseconds(1000);

        bitClear(shifter_1, 1);
        shifterLoad();
        delayMicroseconds(1000);
        break;
      case 2:
        bitSet(shifter_1, 5);
        delayMicroseconds(1000);
        shifterLoad();
        bitClear(shifter_1, 5);
        delayMicroseconds(1000);
        shifterLoad();
        break;
      case 3:
        bitSet(shifter_0, 1);
        delayMicroseconds(1000);
        shifterLoad();
        bitClear(shifter_0, 1);
        delayMicroseconds(1000);
        shifterLoad();
        break;
      case 4:
        bitSet(shifter_0, 5);
        delayMicroseconds(1000);
        shifterLoad();
        bitClear(shifter_0, 5);
        delayMicroseconds(1000);
        shifterLoad();
        break;
    }
  }
  //  shifterReset();
  //  changeSpeed(2);
  //  digitalWrite(LED, LOW);
}

void reverseDirection(int& direction) {
  if (direction == CLOCKWISE) {
    direction = COUNTERCLOCKWISE;
  } else if (direction == COUNTERCLOCKWISE) {
    direction = CLOCKWISE;
  }
}

void moveToOrigin(Steppers stepper, String signal, int steps, int stepsBack, int& maxSteps) {
  int direction = CLOCKWISE;
  String message = "";

  if (stepper == StepperY) {
    reverseDirection(direction);
  }

  do {
    mySerial.write("CHCK", 4);
    if (mySerial.available()) {
      message = mySerial.readString();
      message.trim();
    }
    stepperMove(stepper, direction, steps);
  } while (message.equals(signal) == false);

  reverseDirection(direction);
  stepperMove(stepper, direction, stepsBack);

  if (stepper == StepperZ) {
    maxSteps = MAXSTEPS;
  }

  delay(200);
}

void rotateToOrigin(Steppers stepper, int steps, int stepsBack) {
  int direction = Clockwise;
  int endstopRMinus = 0;

  changeSpeed(8);

  do {
    delayMicroseconds(5000);
    endstopRMinus = digitalRead(ENDSTOP_R_MINUS);
    stepperMove(stepper, direction, steps);
  } while (endstopRMinus == 1);

  reverseDirection(direction);
  stepperMove(stepper, direction, stepsBack);

  delay(200);
}

void moveToPosition(Steppers stepper, unsigned char& spd, int steps, int targetPosition, int& currentPosition) {
  int direction = COUNTERCLOCKWISE;

  changeSpeed(4);
  spd = 4;

  if (stepper == StepperY) {
    reverseDirection(direction);
  }

  if (targetPosition < currentPosition) {
    reverseDirection(direction);
  }

  while (abs(targetPosition - currentPosition) > 10) {
    stepperMove(stepper, direction, steps);
    if (direction == CLOCKWISE) {
      if (stepper == StepperZ) {
        currentPosition -= BIGSTEPS * BIGSTEPS / (spd * 2);
      }
      else if (stepper == StepperY) {
        currentPosition += BIGSTEPS / spd * BIGSTEPS;
      }
      else {
        currentPosition -= BIGSTEPS / spd * BIGSTEPS;
      }
    }
    else {
      if (stepper == StepperZ) {
        currentPosition += BIGSTEPS * BIGSTEPS / (spd * 2);
      }
      else if (stepper == StepperY) {
        currentPosition -= BIGSTEPS / spd * BIGSTEPS;
      }
      else {
        currentPosition += BIGSTEPS / spd * BIGSTEPS;
      }
    }
    Serial.print(targetPosition);
    Serial.print(" ");
    Serial.println(currentPosition);
  }

  delay(200);
}

void rotateToPosition(Steppers stepper, int spd, int steps, float targetPosition, float& currentPosition) {
  int direction = CounterClockwise;
  int endstopRMinus = 0;

  changeSpeed(8);
  if (targetPosition < currentPosition) {
    // targetPosition = -targetPosition;
    reverseDirection(direction);
  }

  while (abs(targetPosition - currentPosition) > 0.2) {
    delayMicroseconds(5000);
    stepperMove(stepper, direction, steps);
    if (targetPosition < currentPosition) {
      currentPosition -= 0.5 / 8 * SMALLSTEPS;
    }
    // Serial.print(" ");
    // Serial.println(currentPosition)
    else {
      currentPosition += 0.5 / 8 * SMALLSTEPS;
    }
    // Serial.print(targetPosition););
  } 

  delay(200);
}
