#include "board.h"

Board::Board(int uid) : Object(uid) {}
Board::Board(Object ob) : Object(ob) {}
Board::Board(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a, int uid) : Object(x, y, wid, hei, i, v, sol, txt, a, uid) {}


Interface Board::interact(Player*, Field*, SwitchHandler*) {
  return Interface(pos, text, "");
}

CacheNodeAttributes Board::draw(const TextureCache* cache) {
  // draw a board with no transforms
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("board");
  //no transforms, so tlist is left blank
  return cna;
}
