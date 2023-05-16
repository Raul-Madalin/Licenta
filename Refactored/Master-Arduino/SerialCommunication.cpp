#include "SerialCommunication.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

SerialCommunication::SerialCommunication(int rxPin, int txPin) : RX(rxPin), TX(txPin), serialCommunication(rxPin, txPin) {
    message = "";
}

void SerialCommunication::Send(char messageSent[]){
  serialCommunication.write(messageSent, 4);
}

String SerialCommunication::Receive() {
  message = "";
  if (serialCommunication.available()) {
    message = serialCommunication.readString();
    message.trim();
  }
  return message;
}