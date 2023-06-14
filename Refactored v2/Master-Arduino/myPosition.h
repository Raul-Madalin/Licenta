struct Position {
  int X, Y, Z;
  float R, radians;
  int radius;
} currentPosition, targetPosition, targetComponent;

void resetPosition(Position& currentPosition) {
  currentPosition.X = 0;
  currentPosition.Y = 0;
  currentPosition.Z = 0;
  currentPosition.R = 0;
}

void printCurrentPosition(int& prevX, int& prevY, int& prevZ, float& prevR) {
  if (prevX != currentPosition.X || prevY != currentPosition.Y || prevZ != currentPosition.Z || prevR != currentPosition.R) {
    // Serial.print("X: ");
    // Serial.print(currentPosition.X);
    // Serial.print(" | ");

    // Serial.print("Y: ");
    // Serial.print(currentPosition.Y);
    // Serial.print(" | ");

    Serial.print("Z: ");
    Serial.print(currentPosition.Z);
    Serial.print(" | ");

    Serial.print("R: ");
    Serial.println(currentPosition.R);

    prevX = currentPosition.X;
    prevY = currentPosition.Y;
    prevZ = currentPosition.Z;
    prevR = currentPosition.R;
  }
}