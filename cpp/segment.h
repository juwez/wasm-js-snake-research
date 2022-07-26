#pragma once

class Segment {
private:
  int x, y; // box location in the game window

public:
  Segment(int x, int y);         // constructor
  void move(int newX, int newY); // jump to a new position
  int getX();
  int getY();
};
