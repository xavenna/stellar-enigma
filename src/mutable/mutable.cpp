#include "mutable/mutable.h"
#include <iostream>

bool Mutable::getSolid() const{
  return solid;
}

sf::Vector2f Mutable::getDelta() const {
  return pos - lastPos;
}

/*
void Mutable::updateDelta() {
  lastPos = savedPos;
}
*/


sf::Vector2f Mutable::getPos() const {
  return pos;
}
sf::Vector2f Mutable::getScaleFactor() const {
  return scaleFactor;
}
sf::Vector2f Mutable::getLastPos() const {
  return lastPos;
}
sf::Vector2i Mutable::getScreen() const {
  return screen;
}
sf::Vector2f Mutable::getSelfPush() const {
  return selfPush;
}

void Mutable::setScaleFactor(sf::Vector2f n) {
  scaleFactor = n;
}
void Mutable::setPos(sf::Vector2f n) {
  pos = n;
}
void Mutable::setLastPos(sf::Vector2f n) {
  lastPos = n;
}
void Mutable::setScreen(sf::Vector2i n) {
  screen = n;
}
void Mutable::setSelfPush(sf::Vector2f n) {
  selfPush = n;
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
void Mutable::setSolid(bool n) {
  solid = n;
}
