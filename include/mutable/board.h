#ifndef BOARD_H
#define BOARD_H

#include "mutable/object.h"

//! An object that displays a message when picked up
class Board : public Object {
public:
  using Object::interact;
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw();
  virtual std::string Name() const {return "board";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(32,32);}
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  Board(int);
};

#endif
