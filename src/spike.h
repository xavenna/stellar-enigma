#ifndef SPIKE_H
#define SPIKE_H
#include "object.h"

//! An object that damages player on contact and blocks movement
class Spike : public Object {
public:
  virtual Interface interact(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  Spike(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Spike();
  Spike(Object);
};

#endif
