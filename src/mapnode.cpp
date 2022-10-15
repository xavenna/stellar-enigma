#include "mapnode.h"

int MapNode::getId() const{
  return id;
}
bool MapNode::getSolid(Direction d) const {
  return solid[d];
}
void MapNode::setId(const int& n) {
  id = n;
}
void MapNode::setSolid(Direction d, bool b) {
  solid.set(d, b);
}
std::string MapNode::getCutname() const {
  return cutname;
}
void MapNode::setCutname(const std::string& n) {
  cutname = n;
}
MapNode::MapNode() {
  id = 0;
  area.setPosition(0,0);
}

MapNode::MapNode(int n, DirectionalBool b, std::string cutn) : id{n}, cutname{cutn}, solid{b} {
}
