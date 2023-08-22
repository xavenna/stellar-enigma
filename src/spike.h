#ifndef SPIKE_H
#define SPIKE_H
#include "solid.h"

//! An object that damages player on contact and blocks movement
class Spike : public Solid {
public:
  using Solid::interact;
  virtual int Type() {return Object::Static;} 
  virtual Interface interact(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual int priority() {return 0;} //0 is highest, decreases counting up
  Spike(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Spike();
  Spike(Object);
};

#endif
