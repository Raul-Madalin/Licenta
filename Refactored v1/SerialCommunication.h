#ifndef SerialCommunication_h
#define SerialCommunication_h

#include <Arduino.h>
#include <SoftwareSerial.h>

class SerialCommunication {
private:
  int RX;
  int TX;
  String message;
  SoftwareSerial serialCommunication;
public:
  SerialCommunication(int rxPin, int txPin);
  void Send(char message[]);
  String Receive();
};

#endif