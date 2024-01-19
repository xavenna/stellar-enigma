#ifndef BOARD_H
#define BOARD_H

#include "object.h"

//! An object that displays a message when picked up
class Board : public Object {
public:
  using Object::interact;
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual std::string Name() {return "board";}
  virtual int Type() {return Object::Intangible;} 
  Board(int);
};

#endif
