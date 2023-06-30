#include "key.h"


Key::Key() : Object() {}
Key::Key(Object ob) : Object(ob) {}
Key::Key(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, txt, a) {}


Interface Key::interact(Player*, Field*, bool) {
  // once keys are a thing that the player can collect, this will increment key count
  status = Destroy;
  return Interface(pos, "", "key");
}

CacheNodeAttributes Key::draw(const TextureCache* cache) {
  // draw a key with no transforms
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("key");
  //no transforms, so tlist is left blank
  return cna;
}
