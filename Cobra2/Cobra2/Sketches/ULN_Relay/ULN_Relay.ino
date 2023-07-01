// switching on and off a relay connected to a LED

void setup() {
  Serial.begin(9600);

  // ULN pin setup
  pinMode(8, OUTPUT);
  
  Serial.println("System online...");
  
}

void loop() {
  Serial.println("LED OFF..");
  digitalWrite(8, HIGH);
  delay(2000);
  
  digitalWrite(8, LOW);
  Serial.println("LED ON...");
  delay(2000);
}
