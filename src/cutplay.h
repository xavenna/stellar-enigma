#ifndef CUTPLAY_H
#define CUTPLAY_H
#include "object.h"

//! An object that plays a cutscene upon contact
class CutPlay : public Object {
public:
  using Object::interact;
  virtual int Type() {return Object::Intangible;} 
  virtual Interface interact(Player*, Field*, bool);
  virtual Interface behave();
  CutPlay(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  CutPlay();
  CutPlay(Object);
};

#endif
