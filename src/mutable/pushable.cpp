#include "mutable/pushable.h"


Pushable::Pushable(int uid) : Object(uid) {}

Interface Pushable::interact(Object* p, Field* l, SwitchHandler*) {
  return Interface();
}

bool Pushable::use(Player* p) {
  pushCounter = 16;
  pushDir = dirToVec(p->getFacing());
  status = Object::Pushed;
  return false;
}

Interface Pushable::behave(SwitchHandler*, Utility*) {
  if(pushCounter > 0) {
    selfPush = pushDir;
    pushCounter--;
  }
  else {
    status = Object::Normal;
    selfPush = zero2<float>();
  }
  return Interface();
}

CacheNodeAttributes Pushable::draw(const TextureCache* cache) {
  // draw a blank tile with no transforms
  CacheNodeAttributes cna;
  switch(args[0]) {
  default:
    cna.srcImg = cache->reverseHash("crate");
  //others can go here for more freedom
  }
  //no transforms, so tlist is left blank
  return cna;
}
