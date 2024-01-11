#include "menu.h"


void Menu::initialize() {
  //select a texture
}


void Menu::assignTexture(TextureCache& cache) {
  CacheNodeAttributes cna;
  cna.srcImg = cache.reverseHash(baseImage);
  cna.tList = tList;
  splash.setTexture(cache.getTexture(cna));
}
