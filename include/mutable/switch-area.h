#ifndef SWITCH_AREA_H
#define SWITCH_AREA_H
#include "mutable/object.h"

//! Switch Area. Activates SW::A if player is currently intersecting with it
class SwArea : public Object {
public:
  Interface interact(Object*, Field*, SwitchHandler*);
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual std::string Name() const {return "switch_area";}
  virtual Interface behave(SwitchHandler*, Utility*);
  SwArea(int uid);
protected:
  bool occupied; //!< Set false at end of behave, is set true if intersection happens
};

#endif
