#ifndef CUTPLAY_H
#define CUTPLAY_H
#include "object.h"

//! An object that plays a cutscene upon contact
class CutPlay : public Object {
public:
  using Object::interact;
  virtual int Type() {return Object::Intangible;} 
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual Interface behave(SwitchHandler*);
  virtual std::string Name() {return "cutscene_player";}
  CutPlay(int);
  CutPlay(Object);
};

#endif
