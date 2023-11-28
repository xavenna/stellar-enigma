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
  Key(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>, int uid);
  Key(int uid);
  Key(Object);
};

#endif
