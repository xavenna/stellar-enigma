#include "mapnode.h"

int MapNode::getId() const{
  return id;
}
void MapNode::setId(const int& n) {
  id = n;
}
std::string MapNode::getCutname() {
  return cutname;
}
void MapNode::setCutname(const std::string& n) {
  cutname = n;
}
MapNode::MapNode() {
  id = 0;
  area.setPosition(0,0);
}

MapNode::MapNode(int n, std::string cutn) : id{n}, cutname{cutn} {
}
