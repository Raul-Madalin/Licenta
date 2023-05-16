#include "SerialCommunication.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

SerialCommunication::SerialCommunication() {
  message = "";
}

void SerialCommunication::send(SoftwareSerial serialCommunication, char messageSent[]){
  serialCommunication.write(messageSent, 4);
}

String SerialCommunication::receive(SoftwareSerial serialCommunication) {
  message = "";
  if (serialCommunication.available()) {
    message = serialCommunication.readString();
    message.trim();
  }
  return message;
}