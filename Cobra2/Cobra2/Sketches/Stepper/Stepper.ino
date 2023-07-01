// controlling a stepper motor using driver and 2 buttons

#define dirPin 2
#define stepPin 3
#define buttonC 4
#define buttonCC 5
#define stepsPerRevolution 200

unsigned char data;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);   
  Serial.print(F("Arduino UNO: Serial_0 is online.")); 
  while(Serial.available())
    data = Serial.read();

  // driver pins
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // button pins
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonCC, INPUT_PULLUP);

}

void loop() 
{
  // button setup
  int buttonValC = !digitalRead(4);
  int buttonValCC = !digitalRead(5);

  if (buttonValC == HIGH) {
    digitalWrite(dirPin, LOW);    // clockwise
    for (int i = 0; i < 30; i++) // TODO: change 30 to parameter value
    {
      // These four lines result in 1 step:
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    Serial.println("Motor engaged clockwise...");
  }

  else if (buttonValCC == HIGH) {
    digitalWrite(dirPin, HIGH);   // counterclockwise
    for (int i = 0; i < 30; i++) // TODO: change 30 to parameter value
    {
      // These four lines result in 1 step:
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
    Serial.println("Motor engaged counterclockwise...");
  }

  else {
    Serial.println("Motor ready...");
  }

  delay(10);
}
