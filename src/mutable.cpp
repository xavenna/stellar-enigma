#include "mutable.h"

int Mutable::getXPos() const{
  return xpos;
}
int Mutable::getYPos() const{
  return ypos;
}
int Mutable::getWidth() const{
  return width;
}
int Mutable::getHeight() const{
  return height;
}
int Mutable::getXScreen() const{
  return xScreen;
}
int Mutable::getYScreen() const{
  return yScreen;
}
bool Mutable::getSolid() const{
  return solid;
}
void Mutable::setXPos(const int& n) {
  xpos = n;
}
void Mutable::setYPos(const int& n) {
  ypos = n;
}
void Mutable::setWidth(const int& n) {
  width = n;
}
void Mutable::setHeight(const int& n) {
  height = n;
}
void Mutable::setXScreen(const int& n) {
  xScreen = n;
}
void Mutable::setYScreen(const int& n) {
  yScreen = n;
}
void Mutable::setSolid(const bool& n) {
  solid = n;
}
