#include "object.h"
#include <iostream>
class Stone;

int Object::getId() const{
  return id;
}
void Object::setId(int n) {
  id = n;
}

int Object::getValue() const{
  return value;
}
void Object::setValue(int n) {
  value = n;
}

bool Object::getCollectable() const{
  return collectable;
}

void Object::setArg(std::size_t slot, int value) {
  if(slot > args.size()) {
    std::cerr << "Error: out-of-bounds object argument assignment\n";
    return;
  }
  args[slot] = value;
}

int Object::getArg(std::size_t slot) const {
  if(slot > args.size()) {
    std::cerr << "Error: out-of-bounds object argument fetch\n";
    return 0;
  }
  return args[slot];
}

std::array<int, 8> Object::getArgs() const {
  return args;
}
void Object::setCollectable(bool n) {
  collectable = n;
}
std::string Object::getText() const {
  return text;
}
void Object::setText(const std::string& n) {
  text = n;
}
Object::Object(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt) : text{txt} {
  pos.x = x;
  pos.y = y;
  size.x = wid;
  size.y = hei;
  id = i;
  value = v;
  solid = sol;
  collectable = col;
}
Object::Object(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8> a) : text{txt} {
  pos.x = x;
  pos.y = y;
  size.x = wid;
  size.y = hei;
  id = i;
  value = v;
  solid = sol;
  collectable = col;
  args = a;
}

Object::Object() {
  pos.x = 0;
  pos.y = 0;
  size.x = 0;
  size.y = 0;
  id = 0;
  solid = false;
  collectable = false;
}


Stone::Stone(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, col, txt, a) {}
Crate::Crate(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, col, txt, a) {}
Board::Board(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, col, txt, a) {}
Spike::Spike(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, col, txt, a) {}
Key::Key(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, col, txt, a) {}

Stone::Stone() : Object() {}
Stone::Stone(Object ob) : Object(ob) {}

Board::Board() : Object() {}
Board::Board(Object ob) : Object(ob) {}

Spike::Spike() : Object() {}
Spike::Spike(Object ob) : Object(ob) {}

Crate::Crate() : Object() {}
Crate::Crate(Object ob) : Object(ob) {}

Key::Key() : Object() {}
Key::Key(Object ob) : Object(ob) {}


std::tuple<sf::Vector2i, std::string, std::string> Object::behave(Player*, Field*, bool) {
  // do absolutely nothing by default
  return std::make_tuple(pos, "", "");
}

std::tuple<sf::Vector2i, std::string, std::string> Crate::behave(Player* p, Field* l, bool) {
  sf::Vector2i pmin{p->getPos()};
  sf::Vector2i pmax{pmin+p->getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{p->getLastPos()};
  sf::Vector2i plmax{plmin+p->getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{pos};
  sf::Vector2i omax{omin+size-sf::Vector2i(1,1)};
  sf::Vector2i olmin{lastPos};
  sf::Vector2i olmax{olmin+size-sf::Vector2i(1,1)};

  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  if(xInt && yInt) {
    return std::make_tuple(pos, "", ""); //For now, diagonal interactions are ignored
  }

  sf::Vector2i residSpeed;
  if(xInt && pmin.x < omin.x) {
    residSpeed.x = p->getPos().x+p->getSize().x-pos.x;
  }  //moving right
  if(yInt && pmin.y < omin.y) {
    residSpeed.y = p->getPos().y+p->getSize().y-pos.y;
  }
  if(xInt && pmin.x > omin.x) {
    residSpeed.x = p->getPos().x - (pos.x+size.x);
  }
  if(yInt && pmin.y > omin.y) {
    residSpeed.y = p->getPos().y - (pos.y+size.y);
  }
  sf::Vector2i moveDistance;

  moveDistance = l->validMove(pos, size, residSpeed);
  // I need to find a way to get this to work, as Level* or MapData* can't be passed in

  pos = pos+moveDistance;
  p->setPos(p->getPos() -(residSpeed - moveDistance));
  return std::make_tuple(pos, "", "");
}

std::tuple<sf::Vector2i, std::string, std::string> Stone::behave(Player* p, Field*, bool dryRun) {
  sf::Vector2i pmin{p->getPos()};
  sf::Vector2i pmax{pmin+p->getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{p->getLastPos()};
  sf::Vector2i plmax{plmin+p->getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{pos};
  sf::Vector2i omax{omin+size-sf::Vector2i(1,1)};
  sf::Vector2i olmin{lastPos};
  sf::Vector2i olmax{olmin+size-sf::Vector2i(1,1)};
  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  if(!dryRun) {
    if(xInt && pmin.x < omin.x) {
      p->setXPos(pos.x-p->getSize().y);
    }
    if(yInt && pmin.y < omin.y) {
      p->setYPos(pos.y-p->getSize().x);
    }
    if(xInt && pmin.x > omin.x) {
      p->setXPos(pos.x+size.x);
    }
    if(yInt && pmin.y > omin.y) {
      p->setYPos(pos.y+size.y);
    }
  }

  if(xInt && pmin.x < omin.x) {
    return std::make_tuple(sf::Vector2i(pos.x-p->getSize().y, p->getPos().y), "", "");
  }
  if(yInt && pmin.y < omin.y) {
    return std::make_tuple(sf::Vector2i(p->getPos().y, pos.y-p->getSize().x), "", "");
  }
  if(xInt && pmin.x > omin.x) {
    return std::make_tuple(sf::Vector2i(pos.x+size.x, p->getPos().y), "", "");
  }
  if(yInt && pmin.y > omin.y) {
    return std::make_tuple(sf::Vector2i(p->getPos().x, pos.y+size.y), "", "");
  }

  return std::make_tuple(p->getPos(), "", "");
}

std::tuple<sf::Vector2i, std::string, std::string> Key::behave(Player*, Field*, bool) {
  return std::make_tuple(pos, "", "key");
}

std::tuple<sf::Vector2i, std::string, std::string> Board::behave(Player*, Field*, bool) {
  return std::make_tuple(pos, text, "");
}
std::tuple<sf::Vector2i, std::string, std::string> Spike::behave(Player* p, Field*, bool) {
  sf::Vector2i pmin{p->getPos()};
  sf::Vector2i pmax{pmin+p->getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{p->getLastPos()};
  sf::Vector2i plmax{plmin+p->getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{pos};
  sf::Vector2i omax{omin+size-sf::Vector2i(1,1)};
  sf::Vector2i olmin{lastPos};
  sf::Vector2i olmax{olmin+size-sf::Vector2i(1,1)};
  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  bool hurt = false;
  if(xInt && pmin.x < omin.x) {
    p->setXPos(pos.x-p->getSize().y);
    hurt = true;
  }
  if(yInt && pmin.y < omin.y) {
    p->setYPos(pos.y-p->getSize().x);
    hurt = true;
  }
  if(xInt && pmin.x > omin.x) {
    p->setXPos(pos.x+size.x);
    hurt = true;
  }
  if(yInt && pmin.y > omin.y) {
    p->setYPos(pos.y+size.y);
    hurt = true;
  }
  if(hurt) {
    if(p->getCooldown() == 0) {
      p->modifyHealth(-1);
      p->damaged = true;
    }
  }
  return std::make_tuple(pos, "", "");
}

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
    return ObjContainer::stone;
  case 1:
    return ObjContainer::crate;
  case 2:
    return ObjContainer::key;
  case 3:
    return ObjContainer::board;
  case 5:
    return ObjContainer::spike;
  default:
    return ObjContainer::obj;
  }
}
bool ObjContainer::storeObj(Object ob, ObjContainer::Type t) {
  switch(t) {
  case ObjContainer::obj:
    list.push_back(new Object(ob));
    break;
  case ObjContainer::stone:
    list.push_back(new Stone(ob));
    break;
  case ObjContainer::crate:
    list.push_back(new Crate(ob));
    break;
  case ObjContainer::key:
    list.push_back(new Key(ob));
    break;
  case ObjContainer::board:
    list.push_back(new Board(ob));
    break;
  case ObjContainer::spike:
    list.push_back(new Spike(ob));
    break;
  default:
    std::clog << "Error: Invalid object type\n";
    return false;
  }
  return true;
}


bool ObjContainer::storeObj(sf::Vector2i pos, sf::Vector2i size, int id, int val, bool s, bool c, const std::string& tex, std::array<int, 8> args, ObjContainer::Type t) {
  switch(t) {
  case ObjContainer::obj:
    list.push_back(new Object(pos.x, pos.y, size.x, size.y, id, val, s, c, tex, args));
    break;
  case ObjContainer::stone:
    list.push_back(new Stone(pos.x, pos.y, size.x, size.y, id, val, s, c, tex, args));
    break;
  case ObjContainer::crate:
    list.push_back(new Crate(pos.x, pos.y, size.x, size.y, id, val, s, c, tex, args));
    break;
  case ObjContainer::key:
    list.push_back(new Key(pos.x, pos.y, size.x, size.y, id, val, s, c, tex, args));
    break;
  case ObjContainer::board:
    list.push_back(new Board(pos.x, pos.y, size.x, size.y, id, val, s, c, tex, args));
    break;
  case ObjContainer::spike:
    list.push_back(new Spike(pos.x, pos.y, size.x, size.y, id, val, s, c, tex, args));
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
  case ObjContainer::stone: 
    list.push_back(new Stone);
    break;
  case ObjContainer::crate:
    list.push_back(new Crate);
    break;
  case ObjContainer::key:
    list.push_back(new Key);
    break;
  case ObjContainer::board:
    list.push_back(new Board);
    break;
  case ObjContainer::spike:
    list.push_back(new Spike);
    break;
  default:
    std::clog << "Error: Invalid object type\n";
    return false;
  }
  return true;
}
