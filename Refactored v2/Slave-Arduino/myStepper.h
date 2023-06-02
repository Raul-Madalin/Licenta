void stepperMove(int dir, int stepsPerRevolution) {
  if (dir == 1)
    digitalWrite(DIR, LOW);
  else
    digitalWrite(DIR, HIGH);

  //  stepsPerRevolution = 10; // TODO: leave as parameter
  for (int count = 0; count < stepsPerRevolution; count++) {
    digitalWrite(STEP, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP, LOW);
    delayMicroseconds(1000);
  }
}
