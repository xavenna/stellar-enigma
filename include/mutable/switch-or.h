#ifndef SWITCH_OR_H
#define SWITCH_OR_H
#include "mutable/object.h"

//! Switch OR. Watches SW::C & SW::D. If either are active, activates SW::A
class SwOr : public Object {
public:
  using Object::interact;
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual std::string Name() const {return "switch_or";}
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual bool verify();
  SwOr(int uid);
};

#endif
