#include "menu.h"


void Menu::initialize() {
  //select a texture
}


void Menu::assignTexture(TextureCache& cache) {
  CacheNodeAttributes cna;
  int s = cache.reverseHash(baseImage);
  if(s < 0) {
    std::cerr << "Error: Invalid texture\n";
    cna.srcImg = 0;
  }
  else {
    cna.srcImg = static_cast<unsigned>(s);
  }
  cna.tList = tList;
  splash.setTexture(cache.getTexture(cna));
}
