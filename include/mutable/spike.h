#ifndef SPIKE_H
#define SPIKE_H
#include "mutable/solid.h"

//! A solid object that damages player upon contact
class Spike : public Solid {
public:
  using Solid::interact;
  virtual int Type() {return Object::Static;} 
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual std::string Name() {return "spike";}
  virtual int priority() {return 0;} //0 is highest, decreases counting up
  Spike(int);
};

#endif
