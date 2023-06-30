#ifndef TOGGLE_BLOCK_H
#define TOGGLE_BLOCK_H
#include "object.h"

//! An object that toggles between two states upon contact
class ToggleBlock : public Object {
public:
  virtual Interface interact(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual Interface behave();
  ToggleBlock(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  ToggleBlock();
  ToggleBlock(Object);
};

#endif
