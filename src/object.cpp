#include "object.h"

int Object::getId() const{
  return id;
}
void Object::setId(const int& n) {
  id = n;
}
bool Object::getCollectable() const{
  return collectable;
}
void Object::setCollectable(const bool& n) {
  collectable = n;
}
std::string Object::getText() const {
  return text;
}
void Object::setText(const std::string& n) {
  text = n;
}
Object::Object(int x, int y, int wid, int hei, int i, bool sol, bool col, const std::string& txt) : text{txt} {
  xpos = x;
  ypos = y;
  width = wid;
  height = hei;
  id = i;
  solid = sol;
  collectable = col;
}

Object::Object() {
  xpos = 0;
  ypos = 0;
  width = 0;
  height = 0;
  id = 0;
  solid = false;
  collectable = false;
}



//some object callback functions:

