#include "board.h"

Board::Board() : Object() {}
Board::Board(Object ob) : Object(ob) {}
Board::Board(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, txt, a) {}


Interface Board::interact(Player*, Field*, bool) {
  return Interface(pos, text, "");
}

CacheNodeAttributes Board::draw(const TextureCache* cache) {
  // draw a board with no transforms
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("board");
  //no transforms, so tlist is left blank
  return cna;
}
