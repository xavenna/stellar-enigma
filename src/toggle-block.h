#ifndef TOGGLE_BLOCK_H
#define TOGGLE_BLOCK_H
#include "solid.h"

//! A solid object that toggles between two states upon contact
class ToggleBlock : public Solid {
public:

  virtual Interface interact(Player*, Field*, SwitchHandler*);
  using Solid::interact;
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave(SwitchHandler*);
  virtual int Type() {return Object::Static;} 
  virtual int priority() {return 0;}
  ToggleBlock(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>, int);
  ToggleBlock(int);
  ToggleBlock(Object);
};

#endif
