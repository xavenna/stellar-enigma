#include "mapnode.h"

NodeBase::NodeBase(unsigned n, DirectionalBool b) : id{n}, solid{b} {
}

unsigned NodeBase::getId() const{
  return id;
}
bool NodeBase::getSolid(Direction d) const {
  return solid[d];
}
void NodeBase::setId(unsigned n) {
  id = n;
}
void NodeBase::setSolid(Direction d, bool b) {
  solid.set(d, b);
}
NodeBase::NodeBase() {
  id = 0;
}

MapNode::MapNode(unsigned n, DirectionalBool b) : NodeBase{n, b} {

}

MapNode::MapNode() : NodeBase{} {
  setPosition(0,0);
  
}
