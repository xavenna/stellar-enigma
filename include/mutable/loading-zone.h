#ifndef LOADING_ZONE_H
#define LOADING_ZONE_H
#include "mutable/object.h"

//! An object that triggers a new map to load.
/*! 
 *  Attributes:
 *  arg0 - trigger mode: 0-triggered by contact. 1-triggered by switch.
 *  arg1 - activation type: 0- loads level specified in text var. 1- switches to mode 4,
 *    using text var as menu name.
 *  arg2 - post-activation: 0- nothing happens. 1-this obj unloads after one activation.
 *  arg3 - cooldown between activations (if multi-use)
 *
 *  SW_A - If arg0==1, setting this switch activates this object
 *
 */
class LoadingZone : public Object {
public:
  using Object::interact;
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() const {return "loading_zone";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(32,32);}
  virtual bool verify();
  LoadingZone(int);
protected:
  int cooldown;
  bool waiting=false;
};



#endif
