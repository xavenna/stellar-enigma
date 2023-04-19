#include "field.h"


NodeBase Field::getNode(int x, int y) const {
  return mapBase[x][y];
}

void Field::updateNode(int x, int y, const NodeBase& node) {
  mapBase[x][y] = node;
}

int Field::getWidth() const {
  return mapBase.size();
}
int Field::getHeight() const {
  return mapBase[0].size();
}

int Field::getTilesizeX() const {
  return tilesize.x;
}
int Field::getTilesizeY() const {
  return tilesize.y;
}
sf::Vector2i Field::getTilesize() const {
  return tilesize;
}

sf::Vector2i Field::validMove(sf::Vector2i pos, sf::Vector2i size, sf::Vector2i speed) const {
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
  return fullMove ? speed : moveDistance;
}

sf::Vector2i Field::loadLevel(const std::string& levelname) {
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
        tilesize.x = std::stoi(line);
        section++;
      }
      else {
    throw -1; //invalid level: non-integer found in integer parameter;
      }
    }
    else if(section == 3) {
      if(isNum(line)) {
        tilesize.y = std::stoi(line);
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
}

Field::Field(size_t x, size_t y) : mapBase{x, std::vector<NodeBase>(y)} {
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
