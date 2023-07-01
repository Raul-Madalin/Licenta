void stepperMove(int stepper, int dir, int stepsPerRevolution) {
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
