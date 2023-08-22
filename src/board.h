#ifndef BOARD_H
#define BOARD_H

#include "object.h"

//! An object that displays a message when picked up
class Board : public Object {
public:
  using Object::interact;
  virtual Interface interact(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual int Type() {return Object::Intangible;} 
  Board(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Board();
  Board(Object);
};

#endif
