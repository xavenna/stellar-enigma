#include <iostream>
#include "icon.h"

sf::Vector2u Icon::getSize() const {
  return size;
}
void Icon::setSize(sf::Vector2u n) {
  size = n;
}
sf::Vector2i Icon::getPos() const {
  return pos;
}
void Icon::setPos(sf::Vector2i n) {
  pos = n;
}
unsigned Icon::getId() const {
  return id;
}
void Icon::setId(unsigned n) {
  id = n;
}
Icon::Icon(std::string call) : id{0} {
  callback = call;
}
PIcon::PIcon(std::string call) : Icon{call} {
}
void Icon::update() {
  //find correct callback
}

void PIcon::update(const Player* pl) {
  //find correct callback
  //run it
  if(callback == "playerHealth") {
    //use showHealthAsNumber
    id = showHealthAsNumber(pl);
  }
  else {
    std::cout << "Error: invalid callback\n";
  }
}


void nullAct() {
  //for static panel objects
  //do nothing
  return;
}

unsigned showHealthAsNumber(const Player* p) {
  //update the passed texture with a file according to a number. If specified file doesn't exist, replace it with a default
  return p->getHealth();
}
