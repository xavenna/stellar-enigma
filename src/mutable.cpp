#include "mutable.h"
#include <iostream>

bool Mutable::getSolid() const{
  return solid;
}

sf::Vector2i Mutable::getDelta() const {
  return pos - lastPos;
}

void Mutable::savePos() {
  savedPos = pos;
}
void Mutable::updateDelta() {
  lastPos = savedPos;
}


sf::Vector2i Mutable::getPos() const {
  return pos;
}
sf::Vector2i Mutable::getLastPos() const {
  return lastPos;
}
sf::Vector2i Mutable::getSize() const {
  return size;
}
sf::Vector2i Mutable::getScreen() const {
  return screen;
}

void Mutable::setPos(sf::Vector2i n) {
  pos = n;
}
void Mutable::setLastPos(sf::Vector2i n) {
  lastPos = n;
}
void Mutable::setSize(sf::Vector2i n) {
  size = n;
}
void Mutable::setScreen(sf::Vector2i n) {
  screen = n;
}

void Mutable::setXPos(const int& n) {
  pos.x = n;
}
void Mutable::setYPos(const int& n) {
  pos.y = n;
}
void Mutable::setPos(const int& m, const int& n) {
  pos.x = m;
  pos.y = n;
}
void Mutable::setWidth(const int& n) {
  size.x = n;
}
void Mutable::setHeight(const int& n) {
  size.y = n;
}
void Mutable::setXScreen(const int& n) {
  screen.x = n;
}
void Mutable::setYScreen(const int& n) {
  screen.y = n;
}
void Mutable::setSolid(const bool& n) {
  solid = n;
}
