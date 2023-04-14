#include "mutable.h"
#include <iostream>

int Mutable::getXPos() const{
  std::clog << "This function xp is deprecated, please use the Vector2 version\n";
  return pos.x;
}
int Mutable::getYPos() const{
  std::clog << "This function yp is deprecated, please use the Vector2 version\n";
  return pos.y;
}
int Mutable::getWidth() const{
  std::clog << "This function sw is deprecated, please use the Vector2 version\n";
  return size.x;
}
int Mutable::getHeight() const{
  std::clog << "This function sh is deprecated, please use the Vector2 version\n";
  return size.y;
}
int Mutable::getXScreen() const{
  std::clog << "This function xs is deprecated, please use the Vector2 version\n";
  return screen.x;
}
int Mutable::getYScreen() const{
  std::clog << "This function ys is deprecated, please use the Vector2 version\n";
  return screen.y;
}
bool Mutable::getSolid() const{
  return solid;
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
