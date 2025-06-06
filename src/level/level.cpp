#include <iostream>
#include <tuple>
#include "level/level.h"

Inter::Inter(Object* m, Object* n) {
  o1 = m;
  o2 = n;
  player1 = false;
  player2 = false;
  priority = 32;
  subpriority = 32;
}

void Inter::calculatePriority() {
  //check if configuration is valid
  //use some formula to assign a priority to the interaction
  if(player1 || player2) {
    //player has the highest priority;
    priority = -1;
    subpriority = o1->priority();
  }
  else {
    priority = (o1->priority() < o2->priority()) ? o1->priority() : o2->priority();
    subpriority = (o1->priority() > o2->priority()) ? o1->priority() : o2->priority();
  }
}


NodeBase Level::getNode(int x, int y) const {
  return field.getNode(static_cast<unsigned>(x),static_cast<unsigned>(y));
}

Object Level::getObj(unsigned index) const {
  return objects.getObj(index);
}
Object& Level::getObjRef(unsigned ind) {
  return objects.getObjRef(ind);
}

Object* Level::getObjPtr(unsigned ind) {
  return objects.getObjPtr(ind);
}

Object* Level::getObjLinkPtr(int ind) {
  return objects.getObjByID(ind);
}

bool Level::hasObj(int id) {
  return objects.hasObj(id);
}

bool Level::hasObjPtr(const Object* ob) {
  return objects.hasObjPtr(ob);
}

bool Level::notifyObj(msg m) {
  return objects.notify(m);
}

void Level::updateNode(int x, int y, const NodeBase& node) {
  field.updateNode(static_cast<unsigned>(x), static_cast<unsigned>(y), node);
}
Level::Level(const size_t& x, const size_t& y) : field{x, y} {
  tilesizeX = 16;  //for now, this is constant, but it may change in the future
  tilesizeY = 16;
}

Level::Level() : field{1, 1} {
  tilesizeX = 16;
  tilesizeY = 16;
}

int Level::loadLevel(const std::string& levelname) {
  // create the levelName
  if(field.loadJsonLevel(levelname)) {
    return -1;
  }
  //initialize walls
  if(!field.initializeWalls()) {

  }
  if(loadMutables(levelname)) {
    return -1;
  }
  return 0;
}
bool Level::loadMutables(const std::string& levelname) {
  objects.clearObjects();
  std::string complevel = "assets/level/" + levelname + ".sml";  //stellar mutable list
  std::ifstream get(complevel);
  std::string line;
  std::string accum;
  Object o;  //these are defined now so they don't have to be every loop
  std::string type;  //Type of requested object
  std::string status; //Status of storeObj
  if(!get.is_open()) {
    return -1;
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line.size() == 0 || line[0] == '#') {
      continue; //skip line
    }
    //parse `line' as a mutable

    std::list<ObjAttr> fields;

    //switch to using: parseSEObjFormat and generateObjFromObjAttrList here
    if(!parseSEObjFormat(line, fields)) {
      std::clog << "Error: Failed level load\n";
      return false;
    }
    generateObjFromObjAttrList(fields, o, type);

    o.setPosition(static_cast<unsigned>(o.getPos().x)+2*tilesizeX, static_cast<unsigned>(o.getPos().y)+2*tilesizeY);
    if(!objects.storeObj(o, type, status)) {
      std::clog << "Error: Failed level load\nMessage: " << status << '\n';
      return false;
    }


  }
  return true;
}

unsigned Level::getWidth() const {
  return field.getWidth();
}
unsigned Level::getHeight() const {
  return field.getHeight();
}
unsigned Level::getTilesizeX() const {
  return field.getTilesizeX();
}
unsigned Level::getTilesizeY() const {
  return field.getTilesizeY();
}
sf::Vector2u Level::getTilesize() const {
  return field.getTilesize();
}
unsigned Level::getObjNum() const {
  return objects.size();
}

int Level::advanceFrameCount() {
  if(frameCount == 1799) {
    frameCount = 0;
    return 0;
  }
  return(++frameCount);

}

void Level::addObject(const Object& ob, const std::string& s) {
  std::string status;
  if(!objects.storeObj(ob, s, status)) {
    std::cerr << status;
  }
}
void Level::removeObject(unsigned index) {
  if(index >= objects.size()) {
    throw std::invalid_argument("Level::removeObject() : Invalid index");
  }
  objects.removeObj(index);
}

void Level::removeObject(Object* ob) {
  objects.removeObj(ob);
}

void Level::resetObjDeltas() {
  for(unsigned i=0;i<objects.size();i++) {
    Object* x = objects.getObjPtr(i);
    x->setLastPos(x->getPos());
  }
}

Interface Level::handleObjects(SwitchHandler* sh, Utility* u) {
  Interface inter;
  std::string status; //status of storeObj()
  for(unsigned i=0;i<objects.size();i++) {
    Object* x = objects.getObjPtr(i);
    //do things for x.
    Interface res = x->behave(sh, u);
    for(auto y : res.message) {
      inter.addMessage(y);
    }
    for(auto y : res.cutscene) {
      inter.playCutscene(y);
    }
    for(auto y : res.sounds) {
      inter.playSound(y);
    }
    if(res.menu != "") {
      inter.openMenu(res.menu); //this means only one menu can be displayed each frame
    }
    if(res.level != "") {
      inter.loadLevel(res.level, res.useMode4);
    }
    for(auto y : res.objs) {
      //create any requested objects

      if(!objects.storeObj(y.first, y.second, status)) {
        std::cerr << status;
      }
    }
    for(auto y : res.notifications) {
      //send any required messages
      objects.notify(y);
    }
    x->setLastPos(x->getPos());
  }

  //remove any destroyed objects
  for(unsigned i=0;i<getObjNum();i++) {
    auto p = getObjPtr(i);
    if(p->getStatus() == Object::Destroy) {
      removeObject(p);
      i--;
    }
  }
  return inter;
}

sf::Vector2f Level::validMove(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f speed) const {
  return field.wallHandle(pos, size, speed);
}

//! Populate an Object's fields from a list of ObjAttr's.
bool generateObjFromObjAttrList(const std::list<ObjAttr>& attribs, Object& obj, std::string& objType) {
  //now that data is extracted from bracketed parts, check for validity
  //if it's valid, assign the appropriate attribute
  //TODO: add a way to avoid duplicates
  std::array<int, 8> args;
  std::array<int, 8> switches;
  sf::Vector2f pos{0,0};
  sf::Vector2f size{0,0};
  sf::Vector2f scale{1,1};
  std::string text;
  std::string objClass;
  int parent=-1;
  int link=-1;
  int tex=-1;
  int argNum;
  for(auto x : attribs) {
    argNum = x.args.size();
    if(x.id == "oa") {
      //assign obj args
      if(argNum > 8) {
        std::clog << "Error: oa field accepts at most 8 arguments, "<<argNum<<" provided\n";
        return false;
      }
      unsigned i=0;
      for(auto y : x.args) {
        args[i++] = y;
      }
    }
    else if(x.id == "sw") {
      //assign switches
      if(argNum > 8) {
        std::clog << "Error: sw field accepts at most 8 arguments, "<<argNum<<" provided\n";
        return false;
      }
      unsigned i=0;
      for(auto y : x.args) {
        switches[i++] = y;
      }
    }
    else if(x.id == "p") {
      //assign position
      if(argNum != 2) {
        std::clog << "Error: p field requires 2 arguments, "<<argNum<<" provided\n";
        return false;
      }
      pos.x = x.args[0];
      pos.y = x.args[1];
    }
    else if(x.id == "s") {
      //assign size
      //are there two int args?
      if(argNum != 2) {
        std::clog << "Error: s field requires 2 argument, "<<argNum<<" provided\n";
        return false;
      }
      size.x = x.args[0];
      size.y = x.args[1];

    }
    else if(x.id == "sc") {
      //assign size
      //are there two int args?
      if(argNum != 2) {
        std::clog << "Error: s field requires 2 argument, "<<argNum<<" provided\n";
        return false;
      }
      scale.x = x.args[0];
      scale.y = x.args[1];

    }
    else if(x.id == "pi") {
      if(argNum != 1) {
        std::clog << "Error: pi field requires 1 argument, "<<argNum<<" provided\n";
        return false;
      }
      //select object type
      parent = x.args[0];
    }
    else if(x.id == "li") {
      if(argNum != 1) {
        std::clog << "Error: li field requires 1 argument, "<<argNum<<" provided\n";
        return false;
      }
      //select object type
      link = x.args[0];
    }
    else if(x.id == "ti") {
      if(argNum != 1) {
        std::clog << "Error: ti field requires 1 argument, "<<argNum<<" provided\n";
        return false;
      }
      //select object type
      tex = x.args[0];
    }
    else if(x.id == "c") {
      if(x.textArg.size() == 0) {
        //null id, fail
        std::clog << "Error: c field requires a non-empty string\n";
        return false;
      }
      objClass = x.textArg;
    }
    else if(x.id == "te") {
      if(x.textArg.size() == 0) {
        //null id, fail
        std::clog << "Error: te field requires a non-empty string\n";
        return false;
      }
      text = x.textArg;
    }
    else {
      //invalid, or text
      std::clog << "Invalid Identifier '"<<x.id<<"' in object list\n";
      return false;
    }
  }
  //now that values have been extracted, assign values to object
  obj.setLinkID(link);
  obj.setParentID(parent);
  obj.setTextureID(tex);
  obj.setArgs(args);
  obj.setSwitches(switches);
  obj.setPos(pos);
  obj.setScaleFactor(scale);
  obj.setText(text);
  obj.setStatus(Object::Normal);
  objType = objClass;
  return true;
}

