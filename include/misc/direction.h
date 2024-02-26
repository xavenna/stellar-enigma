#ifndef DIRECTION_H
#define DIRECTION_H

//! An enum for dealing with cardinal directions
enum Direction {Up=0, Right, Down, Left};


//! A class to simplify and encapsulate directional attributes
/*!
 *  Stores four booleans as bits in an integer. Except when constructing, only
 *  single bits can be changed at once.
 */
class DirectionalBool {
private:
  int bools; //!< Internal buffer

public:
  //! Gets the status of specified direction
  bool operator[] (Direction d) const;
  //! Sets status of specified direction to b
  void set(Direction d, bool b);
  //! Sets the internalbuffer to 0
  DirectionalBool();
  //! Sets the internal buffer to argument & 0xf
  DirectionalBool(int);
};

#endif
