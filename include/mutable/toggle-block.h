#ifndef TOGGLE_BLOCK_H
#define TOGGLE_BLOCK_H
#include "mutable/solid.h"

//! A solid object that toggles between two states upon contact
class ToggleBlock : public Solid {
public:

  virtual Interface interact(Player*, Field*, SwitchHandler*);
  using Solid::interact;
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() {return "toggle_block";}
  virtual int Type() {return Object::Static;} 
  virtual int priority() {return 0;}
  ToggleBlock(int);
};

#endif
