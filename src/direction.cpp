#include "direction.h"

bool DirectionalBool::operator[] (Direction d) const {
  return bools & (1 << d);
}

void DirectionalBool::set(Direction d, bool b) {
  if(b) {
    bools |= (1 << d);
  }
  else {
    bools &= ~(1 << d);
  }
}
DirectionalBool::DirectionalBool() {
  bools = 0xf;
}
