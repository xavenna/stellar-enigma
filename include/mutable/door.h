#ifndef DOOR_H
#define DOOR_H
#include "mutable/object.h"
#include "mutable/solid.h"

//! An object that is solid and blocks movement
class Door : public Solid {
public:
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() const {return "door";}
  virtual int priority() const {return 0;}
  virtual Object::MotionType Type() const {return Object::Static;} 
  virtual bool verify();
  Door(int);
};


#endif
