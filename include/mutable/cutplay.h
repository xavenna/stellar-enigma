#ifndef CUTPLAY_H
#define CUTPLAY_H
#include "mutable/object.h"

//! An object that plays a cutscene upon contact
class CutPlay : public Object {
public:
  using Object::interact;
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() const {return "cutscene_player";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(32,32);}
  CutPlay(int);
};

#endif
