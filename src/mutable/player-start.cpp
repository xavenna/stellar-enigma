#include "mutable/player-start.h"
PlayerStart::PlayerStart(int uid) : Object{uid} {
}


bool PlayerStart::verify() const {
  if(scaleFactor.x == 0 || scaleFactor.y == 0) {
    return false;
  }
  if(args[0] <= 0) {
    return false;
  }
  if(args[1] <= 0) {
    return false;
  }
  if(args[2] <= 0) {
    return false;
  }
  if(args[3] <= 0) {
    return false;
  }
  return true;

}
