#include "stellar.h"

// This module has functions for the basic classes used for the engine

//maybe this should be sorted into files for each type of things?


int Object::getId() const{
  return id;
}
void Object::setId(const int& n) {
  id = n;
}
bool Object::getCollectable() const{
  return collectable;
}
void Object::setCollectable(const bool& n) {
  id = n;
}


int Mutable::getXPos() const{
  return xpos;
}
int Mutable::getYPos() const{
  return ypos;
}
int Mutable::getWidth() const{
  return width;
}
int Mutable::getHeight() const{
  return height;
}
bool Mutable::getSolid() const{
  return solid;
}
void Mutable::setXPos(const int& n) {
  xpos = n;
}
void Mutable::setYPos(const int& n) {
  ypos = n;
}
void Mutable::setWidth(const int& n) {
  width = n;
}
void Mutable::setHeight(const int& n) {
  height = n;
}
void Mutable::setSolid(const bool& n) {
  solid = n;
}

int Entity::getType() const{
  return type;
}
void Entity::setType(const int& n) {
  type = n;
}


bool Message::getPrinted() const {
  return printed;
}
void Message::addMessage(const std::string& str) {
  printQueue.push_back(str);
}
void Message::handleMessages() {
  if(!message.empty()) {
    //there is a message
    //check if 
  }
  //check if a message is currently displayed
}
Message::Message(size_t n, size_t o) : width{n}, maxCool{o} {

}


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

MapNode Level::getNode(const int& x, const int& y) const{
  return mapBase[x][y];
}
void Level::updateNode(const int& x, const int& y, const MapNode& node) {
  mapBase[x][y] = node;
}
Level::Level(const size_t& x, const size_t& y) : mapBase{x, std::vector<MapNode>(y)} {

}
Level::Level() : mapBase{1, std::vector<MapNode>(1)} {
  mapBase.resize(1);
  mapBase[0].resize(1);
}
/*  File Format for Levels, line-by-line view
levelname_m.sel
0-------------------------------------------
1       | unsigned int  :  width of map
2       | unsigned int  :  height of map
3       | unsigned int  :  width of sprites
4       | unsigned int  :  height of sprites
5,height| comma-delimited list  :  nodes of map
 */
int Level::loadLevel(const std::string& levelname) {
  std::string complevel = "assets/level/" + levelname + ".sel";
  std::ifstream load(complevel);
  std::string line;
  int section = 0;
  int column = 0;
  int row = 0;
  while(load.peek() != EOF) {
    std::getline(load, line);
    //parse line
    if(section == 0) {
      if(isNum(line)) {
	mapBase.resize(std::stoi(line));
	section++;
      }
      else {
	throw -1;  //invalid level: non-integer found in integer parameter;
	//error: invalid level
      }
    }
    else if(section == 1) {
      if(isNum(line)) {
	for(auto& m : mapBase) {
	  m.resize(std::stoi(line));
	}
	section++;
      }
      else {
	throw -1; //invalid level: non-integer found in integer parameter;
      }
    }
    else if(section == 2) {
      if(isNum(line)) {
	tilesizeX = std::stoi(line);
	section++;
      }
      else {
	throw -1; //invalid level: non-integer found in integer parameter;
      }
    }
    else if(section == 3) {
      if(isNum(line)) {
	tilesizeY = std::stoi(line);
	section++;
      }
      else {
	throw -1; //invalid level: non-integer found in integer parameter;
      }
    }
    else if(section == 4) {
      column = 0;
      MapNode node;
      //actual level content
      //this line loads into the vector
      //getFront of line
      while(nodify(line, node)) {
	this->updateNode(column, row, node);  //x and y were flipped in order to make maps not load wrongly
	column++;
      }
      row++;
    }
    
  }
  //cleanup things?
  for(size_t i=0;i<mapBase.size();i++) {
    for(size_t j=0;j<mapBase[0].size();j++) {
      mapBase[i][j].area.setPosition(tilesizeX*i,tilesizeY*j);
    }
  }
  loadMutables(levelname);
  return 0;
}
//format:
// 1 char: [noe]; eNtity, Object, or Enemy
//if_o:  oxpos`ypos`width`height`solid?`id`collectable?
void Level::loadMutables(const std::string& levelname) {
  return; /*
  std::string complevel = "assets/level/" + levelname + ".sml";  //stellar mutable list
  std::ifstream get(complevel);
  std::string line;
  std::string accum;
  Entity n;
  Object o;  //these are defined now so they don't have to be every loop
  Enemy e;
  if(!get.is_open()) {
    throw 0;
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    //parse `line' as a mutable
    if(line[0] == 'o') {
      //object
      
    }
    
  }
  return;
	  */
}
int Level::getWidth() const{
  return mapBase.size();
}
int Level::getHeight() const{
  return mapBase[0].size();
}
int Level::getTilesizeX() const{
  return tilesizeX;
}
int Level::getTilesizeY() const{
  return tilesizeY;
}
void Level::assignTextureToNode(const int& x, const int& y, TextureMap& tema) {
  try {
    mapBase.at(x).at(y).area.setTexture(tema.mapping.at(mapBase.at(x).at(y).getId()));
  }
  catch (...) {
    std::cout << "ERROR invalid set texture error.\n";
  }
}

// nodify destroys the passed string, maybe this should be rewritten
bool nodify(std::string& line, MapNode& node) {
  if(line.empty())
    return false;
  std::string accum;
  while(line.size() != 0) {
    char x = line[0];
    if(x == ',') {
      //end of node
      if(!strToNode(accum, node)) {
	throw -3;
      }
      accum.clear();
      line.erase(0,1);
      break;
    }
    else {
      accum += x;
      line.erase(0,1);
    }
  }
  return true;
}
// fields are as follows:  id-int
bool strToNode(const std::string& line, MapNode& node) {
  int field = 0;  
  std::string accum;
  for(auto x : line) {
    if(x == '`') {
      //add accum to appropriate field
      switch(field) {
      case 0:
	if(!isNum(accum)) {
	  return false;
	}
	else {
	  node.setId(std::stoi(accum));
	}
	break;
      default:
	//invalid field
	throw -2;  //error: nonexistent field
	break;
      }
      field++;
    }
    else {
      accum += x;
    }
  }
  return true;
}

sf::Texture& TextureMap::getTexture(const int& n) {
  return mapping.at(n);
}
void TextureMap::assign(int n, sf::Texture tex) {
  try {
    mapping.at(n) = tex;
  }
  catch(...) {
    std::cout << "Error: Invalid\n";
  }
}
bool TextureMap::initialize(const std::string& name) {
  mapping.resize(0);
  std::ifstream get(name);
  std::string line;
  if(!get.is_open()) {
    return false;
  }
  while(get.peek() != EOF) {
    sf::Texture te;
    mapping.push_back(te);
    std::getline(get, line);
    line = "assets/texture/" + line;
    mapping.back().loadFromFile(line);
  }
  return true;
}
int TextureMap::size() const{
  return mapping.size();
}
TextureMap::TextureMap(const std::string& fn) {
  initialize(fn);
}
TextureMap::TextureMap() {
  
}
void Player::initialize() {
  xpos = 0;
  ypos = 0;
  speed = 18;  //this is arbitrary
  width = 36;
  height = 36;
  facingDir = Up;
  picture.loadFromFile("assets/texture/player.png");
  area.setTexture(picture);
}
void Player::update() {
  area.setPosition(xpos, ypos);
}
void Player::setSpeed(int n) {
  speed = n;
}
int Player::getSpeed() const{
  return speed;
}
void Player::setFacing(Direction n) {
  facingDir = n;
}
Direction Player::getFacing() const{
  return facingDir;
}
void Entity::setSpeed(int n) {
  speed = n;
}
int Entity::getSpeed() const{
  return speed;
}


