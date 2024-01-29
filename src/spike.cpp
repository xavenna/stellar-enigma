#include "spike.h"

Spike::Spike(int uid) : Solid(uid) {}

Interface Spike::interact(Player* p, Field* f, SwitchHandler* s) {
  Interface inter = Solid::interact(p, f, s);
  if(interacting) {
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
