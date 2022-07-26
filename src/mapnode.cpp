#include "mapnode.h"

int MapNode::getId() const{
  return id;
}
void MapNode::setId(const int& n) {
  id = n;
}
MapNode::MapNode() {
  id = 0;
  area.setPosition(0,0);
}
