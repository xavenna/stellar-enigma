#include "key.h"


Key::Key(int uid) : Object(uid) {}


Interface Key::interact(Player*, Field*, SwitchHandler* sh) {
  Interface inter;
  // once keys are a thing that the player can collect, this will increment key count
  // trigger key switch
  sh->write(switches[SW::Act1], true);
  status = Destroy;
  inter.playCutscene("key");
  return Interface();
}

CacheNodeAttributes Key::draw(const TextureCache* cache) {
  // draw a key with no transforms
  // use texture_id to decide what to draw
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("key");
  //no transforms, so tlist is left blank
  return cna;
}
