#include "mutable/spike.h"

Spike::Spike(int uid) : Solid(uid) {}


Interface Spike::interact(Object* o, Field* f, SwitchHandler* s) {
  Interface inter = Solid::interact(o, f, s);
  if(o->Type() == Object::Play) {
    Player* p = static_cast<Player*>(o);

    if(p->getCooldown() == 0) {
      p->modifyHealth(-1);
      p->damaged = true;
    }
  }
  return inter;
}

CacheNodeAttributes Spike::draw(const TextureCache* cache) {
  // draw a cactus with no transforms
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("cactus");
  //no transforms, so tlist is left blank
  return cna;
}
