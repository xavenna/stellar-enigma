#include "obj-container.h"

std::size_t ObjContainer::size() const {
  return list.size();
}

ObjContainer::ObjContainer() {

}

ObjContainer::~ObjContainer() {
  for(auto x : list) {
    delete x;
  }
}

Object& ObjContainer::getObjRef(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return *(list[ind]);
}

Object* ObjContainer::getObjPtr(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return (list[ind]);
}
Object ObjContainer::getObj(unsigned ind) const {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  return *(list[ind]);
}

void ObjContainer::removeObj(unsigned ind) {
  if(ind > list.size()) {
    throw std::out_of_range("Invalid array address");
  }
  auto p = list.begin() + ind;
  delete list[ind];
  list.erase(p);
}

ObjContainer::Type getType(unsigned p) {
  switch(p) {
  case 0:
    return ObjContainer::solid;
  case 1:
    return ObjContainer::pushable;
  case 2:
    return ObjContainer::key;
  case 3:
    return ObjContainer::board;
  case 4:
    return ObjContainer::cutscene_player;
  case 5:
    return ObjContainer::spike;
  case 6:
    return ObjContainer::toggle_block;
  default:
    return ObjContainer::obj;
  }
}
bool ObjContainer::storeObj(Object ob, ObjContainer::Type t) {
  switch(t) {
  case ObjContainer::obj:
    list.push_back(new Object(ob));
    break;
  case ObjContainer::solid:
    list.push_back(new Solid(ob));
    break;
  case ObjContainer::pushable:
    list.push_back(new Pushable(ob));
    break;
  case ObjContainer::key:
    list.push_back(new Key(ob));
    break;
  case ObjContainer::board:
    list.push_back(new Board(ob));
    break;
  case ObjContainer::cutscene_player:
    list.push_back(new CutPlay(ob));
    break;
  case ObjContainer::spike:
    list.push_back(new Spike(ob));
    break;
  case ObjContainer::toggle_block:
    list.push_back(new ToggleBlock(ob));
    break;
  default:
    std::clog << "Error: Invalid object type\n";
    return false;
  }
  return true;
}


bool ObjContainer::storeObj(sf::Vector2i pos, sf::Vector2i size, int id, int val, bool s, const std::string& tex, std::array<int, 8> args, ObjContainer::Type t) {
  switch(t) {
  case ObjContainer::obj:
    list.push_back(new Object(pos.x, pos.y, size.x, size.y, id, val, s, tex, args));
    break;
  case ObjContainer::solid:
    list.push_back(new Solid(pos.x, pos.y, size.x, size.y, id, val, s, tex, args));
    break;
  case ObjContainer::pushable:
    list.push_back(new Pushable(pos.x, pos.y, size.x, size.y, id, val, s, tex, args));
    break;
  case ObjContainer::key:
    list.push_back(new Key(pos.x, pos.y, size.x, size.y, id, val, s, tex, args));
    break;
  case ObjContainer::board:
    list.push_back(new Board(pos.x, pos.y, size.x, size.y, id, val, s, tex, args));
    break;
  case ObjContainer::cutscene_player:
    list.push_back(new CutPlay(pos.x, pos.y, size.x, size.y, id, val, s, tex, args));
    break;
  case ObjContainer::spike:
    list.push_back(new Spike(pos.x, pos.y, size.x, size.y, id, val, s, tex, args));
    break;
  case ObjContainer::toggle_block:
    list.push_back(new ToggleBlock(pos.x, pos.y, size.x, size.y, id, val, s, tex, args));
    break;
  default:
    std::clog << "Error: Invalid object type\n";
    return false;
  }
  return true;
}
bool ObjContainer::storeObj(ObjContainer::Type t) {
  switch(t) {
  case ObjContainer::obj:
    list.push_back(new Object);
    break;
  case ObjContainer::solid: 
    list.push_back(new Solid);
    break;
  case ObjContainer::pushable:
    list.push_back(new Pushable);
    break;
  case ObjContainer::key:
    list.push_back(new Key);
    break;
  case ObjContainer::board:
    list.push_back(new Board);
    break;
  case ObjContainer::cutscene_player:
    list.push_back(new CutPlay);
    break;
  case ObjContainer::spike:
    list.push_back(new Spike);
    break;
  case ObjContainer::toggle_block:
    list.push_back(new ToggleBlock);
    break;
  default:
    std::clog << "Error: Invalid object type\n";
    return false;
  }
  return true;
}
