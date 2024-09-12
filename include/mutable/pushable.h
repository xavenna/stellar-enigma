#ifndef PUSHABLE_H
#define PUSHABLE_H
#include "mutable/object.h"
#include "mutable/player.h"

//! An object that can be pushed around, sokoban style (when interacted with)
class Pushable : public Object {
public:
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual bool use(Player*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual std::string Name() const {return "pushable";}
  virtual Object::MotionType Type() const {return Object::Static;} 
  virtual bool grabbable() const {return true;}
  virtual int priority() const {return 2;}
  Pushable(int);
protected:
  unsigned pushCounter=0;
  sf::Vector2f pushDir;
};

#endif
