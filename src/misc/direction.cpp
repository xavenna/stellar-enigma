#include "misc/direction.h"

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

DirectionalBool::DirectionalBool(int n) {
  bools = n & 0xf;
}

sf::Vector2f dirToVec(Direction d) {
  switch(d) {
  case Up:
    return sf::Vector2f(0,-1);
    break;
  case Down:
    return sf::Vector2f(0,1);
    break;
  case Left:
    return sf::Vector2f(-1,0);
    break;
  case Right:
    return sf::Vector2f(1,0);
    break;
  default: //just in case
    return sf::Vector2f(0,0);
    break;
  }
}
