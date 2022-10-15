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
  unsigned counter = 0;
  unsigned section = 0;
  if(!get.is_open()) {
    return false;
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line == "***") { //offset happening
      if(section == 0) {
	//switch to object
	objectOffset = counter;
      }
      else if(section == 1) {
	//switch to entity
	entityOffset = counter;
      }
      section++;
      continue;
    }
    sf::Texture te;
    mapping.push_back(te);
    line = "assets/texture/" + line;
    mapping.back().loadFromFile(line);
    counter++;
  }
  return true;
}
int TextureMap::size() const{
  return mapping.size();
}
int TextureMap::getObjOff() const {
  return objectOffset;
}
int TextureMap::getEntOff() const {
  return entityOffset;
}
TextureMap::TextureMap(const std::string& fn) {
  initialize(fn);
}
TextureMap::TextureMap() {
  
}
