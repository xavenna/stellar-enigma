#ifndef PUSHABLE_H
#define PUSHABLE_H
#include "mutable/object.h"

//! An object that can be pushed around, sokoban style
class Pushable : public Object {
public:
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual std::string Name() const {return "pushable";}
  virtual Object::MotionType Type() const {return Object::Sliding;} 
  virtual int priority() const {return 2;}
  Pushable(int);
};

#endif
