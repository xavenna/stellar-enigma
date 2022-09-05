#ifndef DIRECTION_H
#define DIRECTION_H

//! An enum for dealing with cardinal directions
enum Direction {Up=0, Right, Down, Left};

class DirectionalBool {
  int bools;

public:
  bool operator[] (Direction d) const;
  void set(Direction d, bool b);
  DirectionalBool();
};

#endif
