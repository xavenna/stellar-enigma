#ifndef SOLID_H
#define SOLID_H
#include "mutable/object.h"


//! An object that is solid and blocks movement
class Solid : public Object {
public:
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual std::string Name() const {return "solid";}
  virtual int priority() const {return 0;}
  virtual Object::MotionType Type() const {return Object::Static;} 
  virtual bool verify();
  Solid(int);
};


#endif
