#include "mapnode.h"





NodeBase::NodeBase(int n, DirectionalBool b, const std::string& cutn) : id{n}, cutname{cutn}, solid{b} {
}

int NodeBase::getId() const{
  return id;
}
bool NodeBase::getSolid(Direction d) const {
  return solid[d];
}
void NodeBase::setId(const int& n) {
  id = n;
}
void NodeBase::setSolid(Direction d, bool b) {
  solid.set(d, b);
}
std::string NodeBase::getCutname() const {
  return cutname;
}
void NodeBase::setCutname(const std::string& n) {
  cutname = n;
}
NodeBase::NodeBase() {
  id = 0;
}

MapNode::MapNode(int n, DirectionalBool b, const std::string& cutn) : NodeBase{n, b, cutn} {

}

MapNode::MapNode() : NodeBase{} {
  setPosition(0,0);
  
}
