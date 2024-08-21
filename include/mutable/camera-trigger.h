#ifndef CAMERA_TRIGGER_H
#define CAMERA_TRIGGER_H
#include "mutable/object.h"

//! An object that switches to a specified camera mode upon contact
class CameraTrig : public Object {
public:
  using Object::interact;
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() const {return "camera_trigger";}
  CameraTrig(int);

private:
  unsigned cooldown;
};



#endif
