#include "level/mapnode.h"

NodeBase::NodeBase(unsigned n, DirectionalBool b) : id{n}, solid{b} {
}

unsigned NodeBase::getTileset() const{
  return tileset;
}
bool NodeBase::getSolid(Direction d) const {
  return solid[d];
}
sf::Vector2i NodeBase::getTile() const {
  return tile;
}
void NodeBase::setTileset(unsigned n) {
  tileset = n;
}
void NodeBase::setSolid(Direction d, bool b) {
  solid.set(d, b);
}
void NodeBase::setTile(sf::Vector2i n) {
  tile = n;
}
NodeBase::NodeBase() {
  id = 0;
}

MapNode::MapNode(unsigned n, DirectionalBool b) : NodeBase{n, b} {

}

MapNode::MapNode() : NodeBase{} {
  setPosition(0,0);
  
}

MapNode::MapNode(NodeBase n) : NodeBase{n} {
}
