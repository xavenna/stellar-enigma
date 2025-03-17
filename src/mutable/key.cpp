#include "mutable/key.h"


Key::Key(int uid) : Object(uid) {}


bool Key::verify() {
  // SW::Remove needs to be in the range [0,255]
  return (switches[SW::Remove] >= 0 && switches[SW::Remove] < 256);
}

Interface Key::interact(Object* o, Field*, SwitchHandler* sh) {
  Interface inter;

  if(o->Type() == Object::Play) {
    // once keys are a thing that the player can collect, this will increment key count
    // trigger key switch
    sh->write(switches[SW::Remove], true);
    status = Destroy;
    inter.playCutscene("key");
  }
  return inter;
}

CacheNodeAttributes Key::draw(const TextureCache* cache) {
  // draw a key with no transforms
  // use texture_id to decide what to draw
  CacheNodeAttributes cna;
  cna.srcImg = "key";
  //no transforms, so tlist is left blank
  return cna;
}
