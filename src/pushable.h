#ifndef PUSHABLE_H
#define PUSHABLE_H
#include "object.h"

//! An object that can be pushed around, sokoban style
class Pushable : public Object {
public:
  virtual Interface interact(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  Pushable(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Pushable();
  Pushable(Object);
};

#endif
