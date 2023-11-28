#include "key.h"


Key::Key(int uid) : Object(uid) {}
Key::Key(Object ob) : Object(ob) {
  std::cout << switches[SW::Act1] << '\n';
}
Key::Key(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a, int uid) : Object(x, y, wid, hei, i, v, sol, txt, a, uid) {}


Interface Key::interact(Player*, Field*, SwitchHandler* sh) {
  // once keys are a thing that the player can collect, this will increment key count
  // trigger key switch
  std::cout << switches[SW::Act1] << '\n';
  sh->write(switches[SW::Act1], true);
  status = Destroy;
  return Interface(pos, "", "key");
}

CacheNodeAttributes Key::draw(const TextureCache* cache) {
  // draw a key with no transforms
  // use texture_id to decide what to draw
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("key");
  //no transforms, so tlist is left blank
  return cna;
}
