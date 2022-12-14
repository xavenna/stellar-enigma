#include "level.h"
#include <iostream>

MapNode Level::getNode(const int& x, const int& y) const {
  return mapBase[x][y];
}

Object Level::getObj(int index) const {
  return objectList.at(index);
}

void Level::updateObj(unsigned index, const Object& ob) {
  if(index >= objectList.size()) {
    throw std::invalid_argument("Level::updateObj() : Invalid index");
  }
  objectList.at(index) = ob;
}

Entity Level::getEnt(int index) const {
  return entityList.at(index);
}
void Level::newReadyWindow(int xscr, int yscr) {
  //determine which screen to view
  int xwid = WINDOW_WIDTH-2;
  int ywid = WINDOW_HEIGHT-2;
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
      window[i][j].setPosition(tilesizeX*(i+1),(tilesizeY)*(j+1));
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
    throw 0;
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
      o.setPosition(o.getXPos()+tilesizeX, o.getYPos()+tilesizeY);
      objectList.push_back(o);
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
sf::Vector2i Level::getTilesize() const {
  return sf::Vector2i(tilesizeX, tilesizeY);
}
int Level::getObjNum() const {
  return objectList.size();
}
int Level::getEntNum() const {
  return entityList.size();
}

/*
void Level::assignTextureToNode(const int& x, const int& y, TextureMap& tema) {
  try {
    mapBase.at(x).at(y).setTexture(tema.getTexture(mapBase.at(x).at(y).getId()));
  }
  catch (...) {
    std::cout << "ERROR invalid set texture error.\n";
  }
}
*/

void Level::assignTextureToWinNode(const int& x, const int& y, TextureMap& tema) {
  try {
    if(window.at(x).at(y).getTexture()!=&tema.getTexture(window.at(x).at(y).getId())) {
      window.at(x).at(y).setTexture(tema.getTexture(window.at(x).at(y).getId()));
    }
  }
  catch (...) {
    std::cout << "ERROR invalid set texture error.\n";
  }
}

void Level::assignTextureToObject(int index, TextureMap& tema) {
  try {
    if(objectList.at(index).getTexture() != &tema.getTexture(tema.getObjOff()+objectList.at(index).getId())) {
      objectList.at(index).setTexture(tema.getTexture(tema.getObjOff()+objectList.at(index).getId()));
    }
  }
  catch (...) {
    std::cout << "ERROR invalid set texture error.\n";
  }
}

void Level::assignTextureToEntity(int index, TextureMap& tema) {
  try {
    if(entityList.at(index).getTexture() != &tema.getTexture(tema.getEntOff()+objectList.at(index).getId())) {
      entityList.at(index).setTexture(tema.getTexture(tema.getEntOff()+objectList.at(index).getId()));
    }
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
  objectList.erase(std::next(objectList.begin(),index));
}
void Level::handleEntities() {
  for(unsigned i=0;i<entityList.size();i++) {
    //auto& x = entityList[i];
    //do things for x.
    // If you remove an entity, make sure to do i--;
  }
}

void Level::handleObjects(sf::Vector2i pos, sf::Vector2i size) {
  for(unsigned i=0;i<objectList.size();i++) {
    auto& x = objectList[i];
    //do things for x.

    // int pxRel = (pos.x-size.x)%(size.x*(WINDOW_WIDTH-2))+size.x;
    // int pyRel = (pos.y-size.y)%(size.y*(WINDOW_HEIGHT-2))+size.y;
    
    // int pscrx = (pos.x-pxRel)/((WINDOW_WIDTH-2)*tilesizeX);
    // int pscry = (pos.y-pyRel)/((WINDOW_HEIGHT-2)*tilesizeY);

    sf::Vector2i mid(pos.x+size.x/2, pos.y+size.y/2);
    int pscrx = (mid.x-tilesizeX) / (tilesizeX*(WINDOW_WIDTH-2));
    int pscry = (mid.y-tilesizeY) / (tilesizeY*(WINDOW_HEIGHT-2));
    
    sf::Vector2i omid(x.getXPos()+x.getSize().x/2, x.getPos().y+x.getSize().y/2);
    //calculate objects position on player's screen. If it can be displayed, display it:

    sf::Vector2i relPos(x.getXPos()-(WINDOW_WIDTH-2)*tilesizeX*pscrx+tilesizeX,
			x.getYPos()-(WINDOW_HEIGHT-2)*tilesizeY*pscry+tilesizeY);
    x.setPosition(relPos.x,relPos.y);
    //setPosition((mid.x-tilesize.x)%(tilesize.x*(WINDOW_WIDTH-2))+tilesize.x*2-(width/2), (mid.y-tilesize.y)%(tilesize.y*(WINDOW_HEIGHT-2))+tilesize.y*2-(height/2));

    x.setLastPos(x.getPos());
    // If you remove an object, make sure to do i--;
  }
}
bool Level::displayObject(unsigned index, sf::Vector2i ppos, sf::Vector2i size) const {

  if(objectList.size() <= index) {
    return false;
  }

  Object ob{objectList[index]};
  //don't display invisible objects
  switch(ob.getId()) {
  case 4: //add any other invisible objects here
    return false;
  default:
    break;
  }

  sf::Vector2i mid(ppos.x+size.x/2, ppos.y+size.y/2);

  int pxRel = (mid.x-size.x)%(size.x*(WINDOW_WIDTH-2))+size.x;
  int pyRel = (mid.y-size.y)%(size.y*(WINDOW_HEIGHT-2))+size.y;

  int pscrx = (mid.x-pxRel)/((WINDOW_WIDTH-2)*tilesizeX);
  int pscry = (mid.y-pyRel)/((WINDOW_HEIGHT-2)*tilesizeY);

  //calculate objects position on player's screen. If it can be displayed, display it:

  sf::Vector2i relPos(ob.getXPos()-(WINDOW_WIDTH-2)*tilesizeX*pscrx,
		      ob.getYPos()-(WINDOW_HEIGHT-2)*tilesizeY*pscry);

  if(relPos.x+ob.getSize().x < 0 ||
     relPos.y+ob.getSize().y < 0 ||
     relPos.x + ob.getSize().x > (WINDOW_WIDTH+1)*tilesizeX ||
     relPos.y + ob.getSize().y > (WINDOW_HEIGHT+1)*tilesizeY) {
    return false;
  }
  return true;

}

sf::Vector2i Level::validMove(sf::Vector2i pos, sf::Vector2i size, sf::Vector2i speed, int ignore) const {
  //sorry about how awful this code is

  //convert player coordinates to level coordinates
  int playX = int(pos.x / getTilesizeX());
  int playY = int(pos.y / getTilesizeY());
  int playXg = int((pos.x+size.x-1) / getTilesizeX());
  int playYg = int((pos.y+size.y-1) / getTilesizeY());
  int phx = pos.x + size.x;
  int phy = pos.y + size.y;
  bool fullMove = true;
  sf::Vector2i moveDistance = speed;
  sf::Vector2i deltaR;
  int tempSpeed = 0;

  //find destination square
  
  if(speed.y < 0) {
    //std::cout << speed.x << '\n';
    if(pos.y < speed.y) {
      fullMove = false;
      moveDistance.y = pos.y;
    }
    if(playY != 0) { //if the player isn't at the bottom edge of the map
      int numTiles = int(size.x / getTilesizeX()); //how many tiles to check for collision
      int extraTile = (phx % getTilesizeX() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cxP = pos.x + i * getTilesizeX();
	if(cxP > phx)
	  cxP = phx;
	if(getNode(int(cxP/getTilesizeX()), int(pos.y/getTilesizeY())-1).getSolid(Up)) {
	  tempSpeed = pos.y - playY * getTilesizeY();
	  moveDistance.y = abs(moveDistance.y) > abs(tempSpeed) ? tempSpeed : moveDistance.y;
	}
      }
      fullMove = false;
    }
  }
  else if(speed.y > 0) {
    if(phy + speed.y >= getHeight() * getTilesizeY()) {
      fullMove = false;
      moveDistance.y = getHeight() * getTilesizeY() - phy;
    }
    if(playY != getHeight()) {
      int numTiles = int(size.x / getTilesizeX());
      int extraTile = (phx % getTilesizeX() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cxP = pos.x + i * getTilesizeX();
	if(cxP > phx)
	  cxP = phx;
	if(getNode(int(cxP/getTilesizeX()), int((phy)/getTilesizeY())).getSolid(Down)) {
	  tempSpeed = int(phy / getTilesizeY()) * getTilesizeY() - phy;
	  moveDistance.y = moveDistance.y > tempSpeed ? tempSpeed : moveDistance.y;
	}
      }
      fullMove = false;
    }
  }
  if(speed.x < 0) {
    if(pos.x < speed.x) {
      fullMove = false;
      moveDistance.x = pos.x;
    }
    if(playX != 0) {
      int numTiles = int(size.y / getTilesizeY());
      int extraTile = (phy % getTilesizeY() == 0) ? 0 : 1;
      for(int i=0; i<numTiles+extraTile; i++) {
	int cyP = pos.y + i * getTilesizeY();
	if(cyP > phy)
	  cyP = phy;
	if(getNode(int(pos.x/getTilesizeX())-1, int(cyP/getTilesizeY())).getSolid(Left)) {
	  tempSpeed = pos.x - playX * getTilesizeX();
	  moveDistance.x = abs(moveDistance.x) > abs(tempSpeed) ? tempSpeed : moveDistance.x;
	}
      }
      fullMove = false;
    }
  }
  else if(speed.x > 0) {
    if(phx + speed.x >= getWidth() * getTilesizeX()) {
      fullMove = false;
      moveDistance.x = getWidth() * getTilesizeX() - phx;
    }
    if(playX != getWidth()) {
      int numTiles = int(size.y / getTilesizeY());
      int extraTile = (phy % getTilesizeY() == 0) ? 0 : 1;
      for(int i=0;i<numTiles+extraTile; i++) {
	int cyP = pos.y + i * getTilesizeY();
	if(cyP > phy)
	  cyP = phy;
	if(getNode(int((phx)/getTilesizeX()), int(cyP/getTilesizeY())).getSolid(Right)) {
	  tempSpeed = int(phx / getTilesizeX()) * getTilesizeX() - phx;
	  moveDistance.x = moveDistance.x > tempSpeed ? tempSpeed : moveDistance.x;
	}
      }
      fullMove = false;
    }
  }

  

  sf::Vector2i p = pos + moveDistance;

  int playXn = int(p.x / getTilesizeX());
  int playYn = int(p.y / getTilesizeY());
  int playXgn = int((p.x+size.x-1) / getTilesizeX());
  int playYgn = int((p.y+size.y-1) / getTilesizeY());
  bool colly = false;
  bool collx = false;
  //std::cout << playXn << ' ' << playXgn << '\n';

  if(moveDistance.x < 0 && moveDistance.y < 0) {
    if(playX != playXn) {
      if(getNode(playXn, playYn).getSolid(Left)) {
	collx = true;
      }
    }
    if(playY != playYn) {
      if(getNode(playXn, playYn).getSolid(Up)) {
	colly = true;
      }
    }
  }
  else if(moveDistance.x < 0 && moveDistance.y > 0) {
    if(playX != playXn) {
      if(getNode(playXn, playYgn).getSolid(Left)) {
	collx = true;
      }
    }
    if(playYg != playYgn) {
      if(getNode(playXn, playYgn).getSolid(Down)) {
	colly = true;
      }
    }
  }
  else if(moveDistance.x > 0 && moveDistance.y < 0) {
    if(playXg != playXgn) {
      if(getNode(playXgn, playYn).getSolid(Right)) {
	collx = true;
      }
    }
    if(playY != playYn) {
      if(getNode(playXgn, playYn).getSolid(Up)) {
	colly = true;
      }
    }
  }
  else if(moveDistance.x > 0 && moveDistance.y > 0) {
    if(playXg != playXgn) {
      if(getNode(playXgn, playYgn).getSolid(Right)) {
	collx = true;
      }
    }
    if(playYg != playYgn) {
      if(getNode(playXgn, playYgn).getSolid(Down)) {
	colly = true;
      }
    }
  }

  if(collx)
    moveDistance.x = 0;
  if(colly)
    moveDistance.y = 0;
  //std::cout << moveDistance.x << ' ' << moveDistance.y << ' ' << md2.x << ' ' << md2.y << '\n';


  //check if the resulting move
  //now, iterate through all objects, check if they are solid, and if so,
  //check if they block player's path. If so, shorten move distance.

  //make sure to prevent player becoming trapped inside a solid object

  //just repeat this with entities, if necessary
  sf::Vector2i ts = moveDistance;
  for(int i=0;i<static_cast<int>(objectList.size());i++) {
    auto x = objectList[i];
    if(i == ignore) {
      continue;
    }
    if(!x.getSolid()) {
      continue;
    }
    if(speed.y < 0) {
      if(!(pos.x >= x.getXPos()+x.getWidth() || phx <= x.getXPos())
	 && pos.y >= x.getYPos()+x.getHeight()
	 && pos.y-ts.y < x.getYPos()+x.getHeight()) {
	ts.y = -(x.getYPos()+x.getHeight() - pos.y);
	fullMove = false;
	moveDistance.y = moveDistance.y > ts.y ? ts.y : moveDistance.y;
      }
    }
    if(speed.x > 0) {
      if(!(pos.y >= x.getYPos()+x.getHeight() || phy <= x.getYPos()) && (phx <= x.getXPos() && phx+ts.x > x.getXPos())) {
	ts.x = x.getXPos() - phx;  //the math checks out
	fullMove = false;
	moveDistance.x = moveDistance.x > ts.x ? ts.x : moveDistance.x;
      }
    }
    if(speed.y > 0) {
      if(!(pos.x >= x.getXPos()+x.getWidth() || phx <= x.getXPos()) && (phy <= x.getYPos() && phy+tempSpeed > x.getYPos())) {
	ts.y = x.getYPos() - phy;  //the math checks out
	fullMove = false;
	moveDistance.y = moveDistance.y > ts.x ? ts.x : moveDistance.y;
      }
    }
    if(speed.x < 0) {
      if(!(pos.y >= x.getYPos()+x.getHeight() || phy <= x.getYPos())
	 && pos.x >= x.getXPos()+x.getWidth()
	 && pos.x-ts.x < x.getXPos()+x.getWidth()) {
	ts.x = -(x.getXPos()+x.getWidth() - pos.x);
	fullMove = false;
	moveDistance.x = moveDistance.x > ts.x ? ts.x : moveDistance.x;
      }
      break;
    }
  }
  return fullMove ? speed : moveDistance;
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
