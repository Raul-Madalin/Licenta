#ifndef SerialCommunication_h
#define SerialCommunication_h

#include <Arduino.h>
#include <SoftwareSerial.h>

class SerialCommunication {
private:
  String message;
public:
  SerialCommunication();
  void send(SoftwareSerial serialCommunication, char message[]);
  String receive(SoftwareSerial serialCommunication);
};

#endif