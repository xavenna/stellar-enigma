#ifndef SWITCH_AND_H
#define SWITCH_AND_H
#include "mutable/object.h"

//! Switch AND. Watches SW::C & SW::D. If both are active, activates SW::A
class SwAnd : public Object {
public:
  using Object::interact;
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual std::string Name() const {return "switch_and";}
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual bool verify();
  SwAnd(sf::Texture&, int uid);
};

#endif
