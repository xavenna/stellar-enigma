#include "mutable/mutable.h"
#include <iostream>

bool Mutable::getSolid() const{
  return solid;
}

sf::Vector2f Mutable::getDelta() const {
  return pos - lastPos;
}

void Mutable::savePos() {
  savedPos = pos;
}
void Mutable::updateDelta() {
  lastPos = savedPos;
}


sf::Vector2f Mutable::getPos() const {
  return pos;
}
sf::Vector2f Mutable::getCenter() const {
  return pos + size / 2.f;
}
sf::Vector2f Mutable::getLastPos() const {
  return lastPos;
}
sf::Vector2f Mutable::getSize() const {
  return size;
}
sf::Vector2i Mutable::getScreen() const {
  return screen;
}

void Mutable::setPos(sf::Vector2f n) {
  pos = n;
}
void Mutable::setLastPos(sf::Vector2f n) {
  lastPos = n;
}
void Mutable::setSize(sf::Vector2f n) {
  size = n;
}
void Mutable::setScreen(sf::Vector2i n) {
  screen = n;
}

void Mutable::setXPos(float n) {
  pos.x = n;
}
void Mutable::setYPos(float n) {
  pos.y = n;
}
void Mutable::setPos(float m, float n) {
  pos.x = m;
  pos.y = n;
}
void Mutable::setWidth(float n) {
  size.x = n;
}
void Mutable::setHeight(float n) {
  size.y = n;
}
void Mutable::setXScreen(int n) {
  screen.x = n;
}
void Mutable::setYScreen(int n) {
  screen.y = n;
}
void Mutable::setSolid(bool n) {
  solid = n;
}
