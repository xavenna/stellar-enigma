#include "mutable/board.h"

Board::Board(int uid) : Object(uid) {}


Interface Board::interact(Object* o, Field*, SwitchHandler*) {
  Interface inter;
  if(o->Type() == Object::Play) {
    inter.addMessage(text);
  }
  return inter;

}

CacheNodeAttributes Board::draw(const TextureCache* cache) {
  // draw a board with no transforms
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("board");
  //no transforms, so tlist is left blank
  return cna;
}
