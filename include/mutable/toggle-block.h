#ifndef TOGGLE_BLOCK_H
#define TOGGLE_BLOCK_H
#include "mutable/solid.h"
#include "mutable/player.h"

//! A solid object that toggles between two states upon contact
class ToggleBlock : public Solid {
public:

  virtual bool use(Player*);
  virtual bool verify();
  using Solid::interact;
  virtual CacheNodeAttributes draw();
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() const {return "toggle_block";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(32,32);}
  virtual Object::MotionType Type() const {return Object::Static;} 
  virtual int priority() const {return 0;}
  ToggleBlock(int);
};

#endif
