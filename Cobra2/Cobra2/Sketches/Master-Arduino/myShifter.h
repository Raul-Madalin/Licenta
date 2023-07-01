extern unsigned char shifter_0;
extern unsigned char shifter_1;
extern boolean shifter[16];
extern SoftwareSerial mySerial;

void shifterLoad() {
  digitalWrite(RCLK, LOW);
  shiftOut(SER, SRCLK, MSBFIRST, shifter_0);
  shiftOut(SER, SRCLK, MSBFIRST, shifter_1);
  digitalWrite(RCLK, HIGH);
}

void shifterReset() {
  shifter_0 = 0;
  shifter_1 = 0;
  shifterLoad();
}

void shifterMapping (int stepper, int dir) {  
  switch (stepper) {
    case 1:
      if (dir == 0)
        bitClear(shifter_1, 0);
      if (dir == 1)
        bitSet(shifter_1, 0);
      break;
    case 2:
      if (dir == 0)
        bitClear(shifter_1, 4);
      if (dir == 1)
        bitSet(shifter_1, 4);
      break;
    case 3:
      if (dir == 0)
        bitClear(shifter_0, 0); 
      if (dir == 1)
        bitSet(shifter_0, 0);
      break;
    case 4:
      if (dir == 0)
        bitClear(shifter_0, 4); 
      if (dir == 1)
        bitSet(shifter_0, 4);
      break;
  }
  shifterLoad();
}

void changeSpeed (int spd) {
  Serial.print("Speed: ");Serial.println(spd);
  switch (spd) {
    case 1:
      bitClear(shifter_0, 2);
      bitClear(shifter_0, 3);
      bitClear(shifter_0, 6);
      bitClear(shifter_0, 7);
      bitClear(shifter_1, 2);
      bitClear(shifter_1, 3);
      bitClear(shifter_1, 6);
      bitClear(shifter_1, 7);
      break;
    case 2:
      bitSet(shifter_0, 2);
      bitClear(shifter_0, 3);
      bitSet(shifter_0, 6);
      bitClear(shifter_0, 7);
      bitSet(shifter_1, 2);
      bitClear(shifter_1, 3);
      bitSet(shifter_1, 6);
      bitClear(shifter_1, 7);
      mySerial.write("SPD2");
      break;
    case 4:
      bitClear(shifter_0, 2);
      bitSet(shifter_0, 3);
      bitClear(shifter_0, 6);
      bitSet(shifter_0, 7);
      bitClear(shifter_1, 2);
      bitSet(shifter_1, 3);
      bitClear(shifter_1, 6);
      bitSet(shifter_1, 7);
      mySerial.write("SPD4");
      break;
    case 8:
      bitSet(shifter_0, 2);
      bitSet(shifter_0, 3);
      bitSet(shifter_0, 6);
      bitSet(shifter_0, 7);
      bitSet(shifter_1, 2);
      bitSet(shifter_1, 3);
      bitSet(shifter_1, 6);
      bitSet(shifter_1, 7);
      mySerial.write("SPD8");
      break;
  }
  shifterLoad();
}
