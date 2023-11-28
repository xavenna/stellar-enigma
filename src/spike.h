#ifndef SPIKE_H
#define SPIKE_H
#include "solid.h"

//! A solid object that damages player upon contact
class Spike : public Solid {
public:
  using Solid::interact;
  virtual int Type() {return Object::Static;} 
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual int priority() {return 0;} //0 is highest, decreases counting up
  Spike(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>, int);
  Spike(int);
  Spike(Object);
};

#endif
