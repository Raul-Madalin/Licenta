#ifndef Position_h
#define Position_h

#include <Arduino.h>

class Position {
private:
  int x;
  int y;
  int z;
  int r;

public:
  Position();
  Position(int createdX, int createdY, int createdZ, int createdR);

  void SetPosition(int newX, int newY, int newZ, int newR);
  void SetX(int newX);
  void SetY(int newY);
  void SetZ(int newZ);
  void SetR(int newR);

  int GetX();
  int GetY();
  int GetZ();
  int GetR();
};

#endif