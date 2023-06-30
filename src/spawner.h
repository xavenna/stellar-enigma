#ifndef SPAWNER_H
#define SPAWNER_H

#include "object.h"

//! An object that displays a message when picked up
class Spawner : public Object {
public:
  virtual Interface behave(Player*, Field*, bool);
  Spawner(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Spawner();
  Spawner(Object);
};

#endif
