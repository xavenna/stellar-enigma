#ifndef DIRECTION_H
#define DIRECTION_H
#include <array>
#include <SFML/Graphics.hpp>

//! An enum for dealing with cardinal directions
enum Direction {Up=0, Right, Down, Left};


//! A class to simplify and encapsulate directional attributes
/*!  *  Stores four booleans as bits in an integer. Except when constructing, only
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

template <typename T> class Directional {
public:
  //! Gets the value for a specified direction:
  T operator[] (Direction d) const;
  //! Sets status of specified direction to passed value
  void set(Direction d, T t);
  //! Empty constructor
  Directional();
  //! Sets internal storage to specified array
  Directional(std::array<T, 4> n);

private:
  std::array<T, 4> data;
};


template <typename T> T Directional<T>::operator[] (Direction d) const {
  return data[d];
}
template <typename T> void Directional<T>::set(Direction d, T t) {
  data[d] == t;
}
template <typename T> Directional<T>::Directional() {
  for(auto& x : data) {
    x = T();
  }
}
template <typename T> Directional<T>::Directional(std::array<T, 4> n) {
  data = n;
}

sf::Vector2f dirToVec(Direction d);
      
  

#endif
