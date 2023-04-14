#include "object.h"

int Object::getId() const{
  return id;
}
void Object::setId(int n) {
  id = n;
}

int Object::getValue() const{
  return value;
}
void Object::setValue(int n) {
  value = n;
}

bool Object::getCollectable() const{
  return collectable;
}
void Object::setCollectable(bool n) {
  collectable = n;
}
std::string Object::getText() const {
  return text;
}
void Object::setText(const std::string& n) {
  text = n;
}
Object::Object(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt) : text{txt} {
  pos.x = x;
  pos.y = y;
  size.x = wid;
  size.y = hei;
  id = i;
  value = v;
  solid = sol;
  collectable = col;
}

Object::Object() {
  pos.x = 0;
  pos.y = 0;
  size.x = 0;
  size.y = 0;
  id = 0;
  solid = false;
  collectable = false;
}



//some object callback functions:

