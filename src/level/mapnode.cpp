#include "level/mapnode.h"

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
  tileset = 0;
  tile = sf::Vector2i{0,0};
}

