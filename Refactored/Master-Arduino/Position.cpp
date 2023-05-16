#include "Position.h"

Position::Position(){
  x = 0;
  y = 0;
  z = 0;
  r = 0;
}

Position::Position(int createdX, int createdY, int createdZ, int createdR) {
  x = createdX;
  y = createdY;
  z = createdZ;
  r = createdR;
}

void Position::SetPosition(int newX, int newY, int newZ, int newR) {
  x = newX;
  y = newY;
  z = newZ;
  r = newR;
}
void Position::SetX(int newX) {
  x = newX;
}
void Position::SetY(int newY) {
  y = newY;
}
void Position::SetZ(int newZ) {
  z = newZ;
}
void Position::SetR(int newR) {
  r = newR;
}

int Position::GetX() {
  return x;
}
int Position::GetY() {
  return y;
}
int Position::GetZ() {
  return z;
}
int Position::GetR() {
  return r;
}