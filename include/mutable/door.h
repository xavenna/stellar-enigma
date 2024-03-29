#ifndef DOOR_H
#define DOOR_H
#include "mutable/object.h"
#include "mutable/solid.h"

//! An object that is solid and blocks movement
class Door : public Solid {
public:
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() {return "door";}
  virtual int priority() {return 0;}
  virtual int Type() {return Object::Static;} 
  virtual bool verify();
  Door(int);
};


#endif
