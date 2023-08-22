#ifndef PUSHABLE_H
#define PUSHABLE_H
#include "object.h"

//! An object that can be pushed around, sokoban style
class Pushable : public Object {
public:
  virtual Interface interact(Player*, Field*, bool);
  virtual Interface interact(Object*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual int Type() {return Object::Sliding;} 
  virtual int priority() {return 2;}
  Pushable(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Pushable();
  Pushable(Object);
};

#endif
