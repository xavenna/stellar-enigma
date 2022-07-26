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
  id = n;
}
