#ifndef DOOR_H
#define DOOR_H
#include "mutable/object.h"
#include "mutable/solid.h"

//! An object that is solid and blocks movement, but can be toggled off
/*!
 *  Arguments:
 *  uint args[0]: behavior mode. 1: perma-open; 0: Always Match Switch State; 
 *    2: toggle on rising edge.
 *  SW::A controls door state
 */
class Door : public Solid {
public:
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw();
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() const {return "door";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(32,32);}
  virtual sf::Vector2f Offset() const {return sf::Vector2f(0,0);}
  virtual int priority() const {return 0;}
  virtual Object::MotionType Type() const {return Object::Static;} 
  virtual bool verify();
  Door(int);
protected:
  bool locked;
  bool cooldown;
};


#endif
