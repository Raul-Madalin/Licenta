#include "SerialCommunication.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

SerialCommunication::SerialCommunication(int rxPin, int txPin) : RX(rxPin), TX(txPin), serialCommunication(rxPin, txPin) {
    this->serialCommunication.begin(9600);
    this->message = "";
}

void SerialCommunication::Send(char messageSent[]){
  this->serialCommunication.write(messageSent, 4);
}

String SerialCommunication::Receive() {
  this->message = "";
  if (this->serialCommunication.available() > 0) {
    this->message = this->serialCommunication.readString();
    this->message.trim();
    Serial.println("[STATUS] Received: " + this->message);
  }
  return this->message;
}