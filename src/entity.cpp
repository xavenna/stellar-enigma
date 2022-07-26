#include "entity.h"

int Entity::getType() const{
  return type;
}
void Entity::setType(const int& n) {
  type = n;
}

void Entity::setSpeed(int n) {
  speed = n;
}
int Entity::getSpeed() const{
  return speed;
}
