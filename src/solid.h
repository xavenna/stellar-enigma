#ifndef SOLID_H
#define SOLID_H
#include "object.h"


//! An object that is solid and blocks movement
class Solid : public Object {
public:
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave(SwitchHandler*);
  virtual std::string Name() {return "solid";}
  virtual int priority() {return 0;}
  virtual int Type() {return Object::Static;} 
  Solid(int);
};


#endif
