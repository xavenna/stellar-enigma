#include "level.h"
#include <iostream>

MapNode Level::getNode(const int& x, const int& y) const {
  return mapBase[x][y];
}

Object Level::getObj(int index) const {
  return objectList.at(index);
}

void Level::newReadyWindow(int xpos, int ypos) {
  //determine which screen to view
  int xwid = WINDOW_WIDTH-2;
  int ywid = WINDOW_HEIGHT-2;
  int xscr = int((xpos-1) / xwid);
  int yscr = int((ypos-1) / ywid);
  int xOff = xscr * (xwid);
  int yOff = yscr * (ywid);
  for(int i=0;i<WINDOW_WIDTH;i++) {
    for(int j=0;j<WINDOW_HEIGHT;j++) {
      window[i][j].setId(getNode(i+xOff,j+yOff).getId());
    }
  }
  winOffY = yOff;
  winOffX = xOff;
  displayUpdate = false;
}

//this is kept in case the new thingy breaks and I need to use this instead
void Level::readyWindow(int xScreen, int yScreen) {
  // xScreen and yScreen are the screen coordinates of the player, passed
  // this routine should only be called if the display needs to be updated
  int xOff = xScreen * WINDOW_WIDTH;
  int yOff = yScreen * WINDOW_HEIGHT;
  for(int i=0;i<WINDOW_WIDTH;i++) {
    for(int j=0;j<WINDOW_HEIGHT;j++) {
      window[i][j].setId(getNode(i+xOff,j+yOff).getId());
    }
  }
  //this is much easier than in explor...
  displayUpdate = false;
}
// currently unused

void Level::updateNode(const int& x, const int& y, const MapNode& node) {
  mapBase[x][y] = node;
}
Level::Level(const size_t& x, const size_t& y) : mapBase{x, std::vector<MapNode>(y)} {
  tilesizeX = 36;  //for now, this is constant, but it may change in the future
  tilesizeY = 36;
  updateWindowPos();
}
Level::Level() : mapBase{1, std::vector<MapNode>(1)} {
  tilesizeX = 36;
  tilesizeY = 36;
  updateWindowPos();
}
void Level::updateWindowPos() {
  for(size_t i=0;i<WINDOW_WIDTH;i++) {
    for(size_t j=0;j<WINDOW_HEIGHT;j++) {
      window[i][j].area.setPosition(tilesizeX*(i+1),(tilesizeY)*(j+1));
    }
  }
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
  if(!load.is_open())
    return -8;
  std::string line;
  int section = 0;
  int column = 0;
  int row = 0;
  while(load.peek() != EOF) {
    std::getline(load, line);
    //parse line
    if(line.size() == 0 || (line.size() > 0 && line[0] == '#'))
      continue;
    if(section == 0) {
      if(isNum(line)) {
	mapBase.resize(std::stoi(line));
	section++;
      }
      else {
	throw -1;  //invalid level: non-integer found in integer parameter;
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
  loadMutables(levelname);
  return 0;
}
//format:
// 1 char: [oe]; Object or Entity
//if_o:  oxpos`ypos`width`height`solid?`id`collectable?
void Level::loadMutables(const std::string& levelname) {
  std::string complevel = "assets/level/" + levelname + ".sml";  //stellar mutable list
  std::ifstream get(complevel);
  std::string line;
  std::string accum;
  Entity n;
  Object o;  //these are defined now so they don't have to be every loop
  if(!get.is_open()) {
    throw 0;
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line.size() == 0 || line[0] == '#') {
      continue; //skip line
    }
    if(line.back() != '`')
      line += '`';
    //parse `line' as a mutable
    if(line[0] == 'o') {
      //object
      str2obj(line.substr(1), o);
      o.area.setPosition(o.getXPos()+tilesizeX, o.getYPos()+tilesizeY);
      objectList.push_back(o);
      //std::cout << o.getId()<<'\n';
      //add o to object list
    }
    //the others will be written once objects work
    
  }
  return;
}
int Level::getWidth() const {
  return mapBase.size();
}
int Level::getHeight() const {
  return mapBase[0].size();
}
int Level::getTilesizeX() const {
  return tilesizeX;
}
int Level::getTilesizeY() const {
  return tilesizeY;
}
int Level::getObjNum() const {
  return objectList.size();
}
int Level::getEntNum() const {
  return entityList.size();
}

void Level::assignTextureToNode(const int& x, const int& y, TextureMap& tema) {
  try {
    mapBase.at(x).at(y).area.setTexture(tema.mapping.at(mapBase.at(x).at(y).getId()));
  }
  catch (...) {
    std::cout << "ERROR invalid set texture error.\n";
  }
}

void Level::assignTextureToWinNode(const int& x, const int& y, TextureMap& tema) {
  try {
    window.at(x).at(y).area.setTexture(tema.mapping.at(window.at(x).at(y).getId()));
  }
  catch (...) {
    std::cout << "ERROR invalid set texture error.\n";
  }
}

void Level::assignTextureToObject(int index, TextureMap& tema) {
  try {
    objectList.at(index).area.setTexture(tema.mapping.at(tema.getObjOff()+objectList.at(index).getId()));
  }
  catch (...) {
    std::cout << "ERROR invalid set texture error.\n";
  }
}

void Level::assignTextureToEntity(int index, TextureMap& tema) {
  try {
    entityList.at(index).area.setTexture(tema.mapping.at(tema.getEntOff()+objectList.at(index).getId()));
  }
  catch (...) {
    std::cout << "ERROR invalid set texture error.\n";
  }
}

void Level::addEntity(const Entity& en) {
  entityList.push_back(en);
}
void Level::removeEntity(unsigned index) {
  if(index >= entityList.size()) {
    throw std::invalid_argument("Level::removeEntity() : Invalid index");
  }
  entityList.erase(entityList.begin()+index);
}
void Level::addObject(const Object& ob) {
  objectList.push_back(ob);
}
void Level::removeObject(unsigned index) {
  if(index >= objectList.size()) {
    throw std::invalid_argument("Level::removeObject() : Invalid index");
  }
  objectList.erase(objectList.begin()+index);
}
void Level::handleEntities() {
  for(unsigned i=0;i<entityList.size();i++) {
    auto& x = entityList[i];
    //do things for x.
    // If you remove an entity, make sure to do i--;
  }
}

void Level::handleObjects() {
  for(unsigned i=0;i<objectList.size();i++) {
    auto& x = objectList[i];
    //do things for x.

    x.area.setPosition((x.getXPos()-x.getWidth())%(x.getWidth()*(WINDOW_WIDTH-2))+x.getWidth()+36, (x.getYPos()-x.getHeight())%(x.getHeight()*(WINDOW_HEIGHT-2))+x.getHeight()+36);
    // If you remove an object, make sure to do i--;
    // update display position
  }
}
bool Level::displayObject(unsigned index, sf::Vector2i ppos, sf::Vector2i size) const {
  int pxRel = (ppos.x-size.x)%(size.x*(WINDOW_WIDTH-2))+size.x;
  int pyRel = (ppos.y-size.y)%(size.y*(WINDOW_HEIGHT-2))+size.y;

  int pscrx = (ppos.x-pxRel)/((WINDOW_WIDTH-2)*tilesizeX);
  int pscry = (ppos.y-pyRel)/((WINDOW_HEIGHT-2)*tilesizeY);

  int xMin = pscrx*WINDOW_WIDTH*tilesizeX;
  int yMin = pscry*WINDOW_HEIGHT*tilesizeY;
  int xMax = xMin + (WINDOW_WIDTH*tilesizeX);
  int yMax = yMin + (WINDOW_HEIGHT*tilesizeY);


  if(objectList.size() <= index) {
    std::cout << "bad\n";
    return false;
  }

  if(objectList[index].getXPos() >= xMin &&
     objectList[index].getYPos() >= yMin &&
     objectList[index].getXPos() < xMax &&
     objectList[index].getYPos() < yMax) {
    return true;
  }
  return false;
}

int Level::validMove(Player& player) const {
  //this needs to be modified to handle entities

  //sorry about how awful this code is

  //convert player coordinates to level coordinates
  int playX = int(player.getXPos() / getTilesizeX());
  int playY = int(player.getYPos() / getTilesizeY());
  int phx = player.getXPos() + player.getWidth();
  int phy = player.getYPos() + player.getHeight();
  bool fullMove = true;
  int moveDistance = player.getSpeed();
  int tempSpeed = player.getSpeed();

  //find destination square
  switch(player.getFacing()) {
  case Up:
    if(player.getYPos() - player.getSpeed() < 0) {
      fullMove = false;
      moveDistance = player.getYPos();
    }
    if(playY != 0) {
      int numTiles = int(player.getWidth() / getTilesizeX());
      int extraTile = (phx % getTilesizeX() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cxP = player.getXPos() + i * getTilesizeX();
	if(cxP > phx)
	  cxP = phx;
	if(getNode(int(cxP/getTilesizeX()), int(player.getYPos()/getTilesizeY())-1).getSolid(Up)) {
	  tempSpeed = player.getYPos() - playY * getTilesizeY();
	  moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
	}
      }
      fullMove = false;
    }
    //test object collision
    break;
  case Right:
    if(phx + player.getSpeed() >= getWidth() * getTilesizeX()) {
      fullMove = false;
      moveDistance = getWidth() * getTilesizeX() - phx;
    }
    if(playX != getWidth()) {
      int numTiles = int(player.getHeight() / getTilesizeY());
      int extraTile = (phy % getTilesizeY() == 0) ? 0 : 1;
      for(int i=0;i<numTiles+extraTile; i++) {
	int cyP = player.getYPos() + i * getTilesizeY();
	if(cyP > phy)
	  cyP = phy;
	if(getNode(int((phx)/getTilesizeX()), int(cyP/getTilesizeY())).getSolid(Right)) {
	  tempSpeed = int(phx / getTilesizeX()) * getTilesizeX() - phx;
	  moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
	}
      }
      fullMove = false;
    }
    break;
  case Down:
    if(phy + player.getSpeed() >= getHeight() * getTilesizeY()) {
      fullMove = false;
      moveDistance = getHeight() * getTilesizeY() - phy;
    }
    if(playY != getHeight()) {
      int numTiles = int(player.getWidth() / getTilesizeX());
      int extraTile = (phx % getTilesizeX() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cxP = player.getXPos() + i * getTilesizeX();
	if(cxP > phx)
	  cxP = phx;
	if(getNode(int(cxP/getTilesizeX()), int((phy)/getTilesizeY())).getSolid(Down)) {
	  tempSpeed = int(phy / getTilesizeY()) * getTilesizeY() - phy;
	  moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
	}
      }
      fullMove = false;
    }
    break;
  case Left:
    if(player.getXPos() - player.getSpeed() < 0) {
      fullMove = false;
      moveDistance = player.getXPos();
    }
    if(playX != 0) {
      int numTiles = int(player.getHeight() / getTilesizeY());
      int extraTile = (phy % getTilesizeY() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cyP = player.getYPos() + i * getTilesizeY();
	if(cyP > phy)
	  cyP = phy;
	if(getNode(int(player.getXPos()/getTilesizeX())-1, int(cyP/getTilesizeY())).getSolid(Left)) {
	  tempSpeed = player.getXPos() - playX * getTilesizeX();
	  moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
	}
      }
      fullMove = false;
    }
    break;
  }

  //now, iterate through all objects, check if they are solid, and if so,
  //check if they block player's path. If so, shorten move distance.

  //make sure to prevent player becoming trapped inside a solid object

  //just repeat this with entities, if necessary
  tempSpeed = moveDistance;
  for(auto x : objectList) {
    if(!x.getSolid()) {
      continue;
    }
    switch (player.getFacing()) {
    case Up:
      if(!(player.getXPos() >= x.getXPos()+x.getWidth() || phx <= x.getXPos())
	 && player.getYPos() >= x.getYPos()+x.getHeight()
	 && player.getYPos()-tempSpeed < x.getYPos()+x.getHeight()) {
	tempSpeed = -(x.getYPos()+x.getHeight() - player.getYPos());
	fullMove = false;
	moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
      }
      break;
    case Right:
      if(!(player.getYPos() >= x.getYPos()+x.getHeight() || phy <= x.getYPos()) && (phx <= x.getXPos() && phx+tempSpeed > x.getXPos())) {
	tempSpeed = x.getXPos() - phx;  //the math checks out
	fullMove = false;
	moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
      }
      break;
    case Down:
      if(!(player.getXPos() >= x.getXPos()+x.getWidth() || phx <= x.getXPos()) && (phy <= x.getYPos() && phy+tempSpeed > x.getYPos())) {
	tempSpeed = x.getYPos() - phy;  //the math checks out
	fullMove = false;
	moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
      }
      break;
    case Left:
      if(!(player.getYPos() >= x.getYPos()+x.getHeight() || phy <= x.getYPos())
	 && player.getXPos() >= x.getXPos()+x.getWidth()
	 && player.getXPos()-tempSpeed < x.getXPos()+x.getWidth()) {
	tempSpeed = -(x.getXPos()+x.getWidth() - player.getXPos());
	fullMove = false;
	moveDistance = moveDistance > tempSpeed ? tempSpeed : moveDistance;
      }
      break;
    }
  }
  return fullMove ? player.getSpeed() : moveDistance;
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
      case 1:
	if(!isNum(accum) || std::stoi(accum) < 0 || std::stoi(accum) > 15) {
	  return false;
	}
	node.setSolid(Up, std::stoi(accum) & 1);
	node.setSolid(Right, !!((std::stoi(accum) & 2) >> 1));
	node.setSolid(Down, !!((std::stoi(accum) & 4) >> 2));
	node.setSolid(Left, !!((std::stoi(accum) & 8) >> 3));
	break;
      case 2:
	node.setCutname(accum);
	break;
      default:
	//invalid field
	throw -2;  //error: nonexistent field
	break;
      }
      field++;
      accum.clear();
    }
    else {
      accum += x;
    }
  }
  return true;
}

bool str2obj(const std::string& line, Object& obj) {
  //#oxpos`ypos`width`height`solid?`id`collectable?`text
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
	  obj.setXPos(std::stoi(accum));
	}
	break;
      case 1:
	if(!isNum(accum)) {
	  return false;
	}
	else {
	  obj.setYPos(std::stoi(accum));
	}
	break;
      case 2:
	if(!isNum(accum)) {
	  return false;
	}
	else {
	  obj.setWidth(std::stoi(accum));
	}
	break;
      case 3:
	if(!isNum(accum)) {
	  return false;
	}
	else {
	  obj.setHeight(std::stoi(accum));
	}
	break;
      case 4:
	//this should be a bool
	if(!isBool(accum)) {
	  return false;
	}
	else {
	  obj.setSolid(std::stoi(accum));
	}
	break;
      case 5:
	if(!isNum(accum)) {
	  return false;
	}
	else {
	  obj.setId(std::stoi(accum));
	}
	break;
      case 6:
	if(!isBool(accum)) {
	  return false;
	}
	else {
	  obj.setCollectable(std::stoi(accum));
	}
	break;
      case 7:
	obj.setText(accum);
	break;
      default:
	//invalid field
	throw -2;  //error: nonexistent field
	break;
      }
      field++;
      accum.clear();
    }
    else {
      accum += x;
    }
  }
  return true;
}
