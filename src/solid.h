#ifndef SOLID_H
#define SOLID_H
#include "object.h"


//! An object that is solid and blocks movement
class Solid : public Object {
public:
  virtual Interface interact(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave();
  Solid(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Solid();
  Solid(Object);
};


#endif
