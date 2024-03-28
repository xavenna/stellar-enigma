#ifndef SWITCH_AND_H
#define SWITCH_AND_H
#include "mutable/object.h"

//! Switch AND. Watches SW::Trig1 & SW::Trig2. If both are active, activates SW::Act1
class SwAnd : public Object {
public:
  using Object::interact;
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual Interface behave(SwitchHandler*, Utility*);
  SwAnd(int uid);
};

#endif
