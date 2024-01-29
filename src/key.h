#ifndef KEY_H
#define KEY_H
#include "object.h"

//! An object that can be picked up, plays cutscene 'key' when picked up
class Key : public Object {
public:
  using Object::interact;
  virtual int Type() {return Object::Intangible;} 
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual std::string Name() {return "key";}
  virtual bool verify();
  Key(int uid);
};

#endif
