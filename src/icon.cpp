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
    showHealthAsNumber(pl, tex);
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

void showHealthAsNumber(const Player* p, sf::Texture& t) {
  //update the passed texture with a file according to a number. If specified file doesn't exist, replace it with a default
  switch(p->getHealth()) {
  case 1:
    t.loadFromFile("assets/interface/hp-1.png");
    break;
  case 2:
    t.loadFromFile("assets/interface/hp-2.png");
    break;
  case 3:
    t.loadFromFile("assets/interface/hp-3.png");
    break;
  case 4:
    t.loadFromFile("assets/interface/hp-4.png");
    break;
  case 5:
    t.loadFromFile("assets/interface/hp-5.png");
    break;
  default:
    t.loadFromFile("assets/interface/hp-blank.png");
    break;
  }
}
