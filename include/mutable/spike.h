#ifndef SPIKE_H
#define SPIKE_H
#include "mutable/solid.h"
#include "mutable/player.h"

//! A solid object that damages player upon contact
class Spike : public Solid {
public:
  using Solid::interact;
  virtual Object::MotionType Type() const {return Object::Static;} 
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual std::string Name() const {return "spike";}
  virtual int priority() const {return 0;} //0 is highest, decreases counting up
  Spike(int);
};

#endif
