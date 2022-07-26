#include "texturemap.h"

sf::Texture& TextureMap::getTexture(const int& n) {
  return mapping.at(n);
}
void TextureMap::assign(int n, sf::Texture tex) {
  try {
    mapping.at(n) = tex;
  }
  catch(...) {
    std::cout << "Error: Invalid\n";
  }
}
bool TextureMap::initialize(const std::string& name) {
  mapping.resize(0);
  std::ifstream get(name);
  std::string line;
  if(!get.is_open()) {
    return false;
  }
  while(get.peek() != EOF) {
    sf::Texture te;
    mapping.push_back(te);
    std::getline(get, line);
    line = "assets/texture/" + line;
    mapping.back().loadFromFile(line);
  }
  return true;
}
int TextureMap::size() const{
  return mapping.size();
}
TextureMap::TextureMap(const std::string& fn) {
  initialize(fn);
}
TextureMap::TextureMap() {
  
}
