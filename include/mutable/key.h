#ifndef KEY_H
#define KEY_H
#include "mutable/object.h"

//! An object that can be picked up, plays cutscene 'key' when picked up
class Key : public Object {
public:
  using Object::interact;
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw();
  virtual std::string Name() const {return "key";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(32,32);}
  virtual bool verify();
  Key(int uid);
};

#endif
