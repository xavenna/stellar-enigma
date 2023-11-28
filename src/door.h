#ifndef DOOR_H
#define DOOR_H
#include "object.h"
#include "solid.h"

//! An object that is solid and blocks movement
class Door : public Solid {
public:
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave(SwitchHandler*);
  virtual int priority() {return 0;}
  virtual int Type() {return Object::Static;} 
  Door(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>, int);
  Door(int);
  Door(Object);
};


#endif
