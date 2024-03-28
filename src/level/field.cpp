#include "level/field.h"


NodeBase Field::getNode(unsigned x, unsigned y) const {
  return mapBase[x][y];
}

void Field::updateNode(unsigned x, unsigned y, const NodeBase& node) {
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

/*
sf::Vector2f Field::wallPush(const Object* o, sf::Vector2f move) {
  return validMove(o->getPos(), o->getSize(), move) - move;
}
*/
sf::Vector2f Field::validMove(sf::Vector2f pos, sf::Vector2f size, sf::Vector2f speed) const {
  //convert player coordinates to level coordinates
  int playX = int(pos.x / getTilesizeX());
  int playY = int(pos.y / getTilesizeY());
  int playXg = int((pos.x+size.x-1) / getTilesizeX());
  int playYg = int((pos.y+size.y-1) / getTilesizeY());
  int phx = pos.x + size.x;
  int phy = pos.y + size.y;
  bool fullMove = true;
  sf::Vector2f moveDistance = speed;
  sf::Vector2f deltaR;
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
        if(getNode(unsigned(cxP/getTilesizeX()), unsigned(pos.y/getTilesizeY())-1).getSolid(Down)) {
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
        if(getNode(unsigned(cxP/getTilesizeX()), unsigned((phy)/getTilesizeY())).getSolid(Up)) {
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
        if(getNode(unsigned(pos.x/getTilesizeX())-1, unsigned(cyP/getTilesizeY())).getSolid(Right)) {
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
        if(getNode(unsigned((phx)/getTilesizeX()), unsigned(cyP/getTilesizeY())).getSolid(Left)) {
          tempSpeed = int(phx / getTilesizeX()) * getTilesizeX() - phx;
          moveDistance.x = moveDistance.x > tempSpeed ? tempSpeed : moveDistance.x;
        }
      }
      fullMove = false;
    }
  }



  sf::Vector2f p = pos + moveDistance;

  int playXn = int(p.x / getTilesizeX());
  int playYn = int(p.y / getTilesizeY());
  int playXgn = int((p.x+size.x-1) / getTilesizeX());
  int playYgn = int((p.y+size.y-1) / getTilesizeY());
  bool colly = false;
  bool collx = false;

  if(moveDistance.x < 0 && moveDistance.y < 0) {
    if(playX != playXn) {
      if(getNode(static_cast<unsigned>(playXn), static_cast<unsigned>(playYn)).getSolid(Left)) {
        collx = true;
      }
    }
    if(playY != playYn) {
      if(getNode(static_cast<unsigned>(playXn), static_cast<unsigned>(playYn)).getSolid(Up)) {
        colly = true;
      }
    }
  }
  else if(moveDistance.x < 0 && moveDistance.y > 0) {
    if(playX != playXn) {
      if(getNode(static_cast<unsigned>(playXn), static_cast<unsigned>(playYn)).getSolid(Left)) {
        collx = true;
      }
    }
    if(playYg != playYgn) {
      if(getNode(static_cast<unsigned>(playXn), static_cast<unsigned>(playYn)).getSolid(Down)) {
        colly = true;
      }
    }
  }
  else if(moveDistance.x > 0 && moveDistance.y < 0) {
    if(playXg != playXgn) {
      if(getNode(static_cast<unsigned>(playXn), static_cast<unsigned>(playYn)).getSolid(Right)) {
        collx = true;
      }
    }
    if(playY != playYn) {
      if(getNode(static_cast<unsigned>(playXn), static_cast<unsigned>(playYn)).getSolid(Up)) {
        colly = true;
      }
    }
  }
  else if(moveDistance.x > 0 && moveDistance.y > 0) {
    if(playXg != playXgn) {
      if(getNode(static_cast<unsigned>(playXn), static_cast<unsigned>(playYn)).getSolid(Right)) {
        collx = true;
      }
    }
    if(playYg != playYgn) {
      if(getNode(static_cast<unsigned>(playXn), static_cast<unsigned>(playYn)).getSolid(Down)) {
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

int Field::loadJsonLevel(const std::string& levelname) {
  std::string complevel = "assets/level/" + levelname + ".json";
  std::ifstream read(complevel);
  std::string data;
  std::string err;
  if(!read.is_open()) {
    std::clog << "Error: Could not open save file\n";
    return -1;
  }
  getEntireFile(read, data);
  read.close();
  //read contents of save file
  json11::Json save;
  save = save.parse(data, err);
  if(!err.empty()) {
    std::clog << "Json parse error: '"<<err<<"'\n";
    return -2;
  }

  sf::Vector2i mapSize; // (units of nodes)
  sf::Vector2i tileSize; // in pixels

  //parse 'save', extract level data

  json11::Json::object k = save.object_items();


  json11::Json::array layers = k["layers"].array_items();
  if(k.size() < 5) {
    std::cerr << "Error: missing layer data\n";

  }
  for(auto x : layers) {
    //parse the layer
    //first get title
    //next determine tilesize and map size

    std::string name = x["name"].string_value();
    sf::Vector2i map = sf::Vector2i(x["gridCellsX"].int_value(), x["gridCellsY"].int_value());
    sf::Vector2i tile = sf::Vector2i(x["gridCellWidth"].int_value(), x["gridCellHeight"].int_value());

    std::vector<json11::Json> data = x["dataCoords2D"].array_items();
    std::vector<json11::Json> gridData = x["grid2D"].array_items();

    if(name == "Base Layer") {
      std::string tileset = x["tileset"].string_value();
      //set size of mapbase appropriately
      mapBase.resize(map.x);
      for(auto& k : mapBase) {k.resize(map.y);}

      mapSize = map;
      tileSize = tile;
      //populate nodes with data:
      //ensure array size equals gridCells size
      for(int i=0;i<data.size();i++) {
        std::vector<json11::Json> row = data[i].array_items();
        for(int j=0;j<row.size();j++) {
          json11::Json elem = row[j].array_items();
          mapBase[j][i].setTile(sf::Vector2i(elem[0].int_value(), elem[1].int_value()));
          mapBase[j][i].setTileset(0); //this should be set from tileset somehow
          // i signifies the row, j signifies the column. As the level data is stored
          // as (x,y), this needs to be flipped
        }
      }
    }
    else if(name == "Left Solidity" || name == "Right Solidity" || name == "Top Solidity" || name == "Bottom Solidity") {
      Direction d;
      if(name == "Left Solidity") {
        d = Left;
      }
      else if(name == "Right Solidity") {
        d = Right;
      }
      else if(name == "Top Solidity") {
        d = Up;
      }
      else if(name == "Bottom Solidity") {
        d = Down;
      }
      if(map != mapSize) {
        std::cerr << "Error: mismatching layer size\n";
        return 1;
      }
      if(gridData.size() < map.y) {
        std::cerr << "Error: Solidity layer data size mismatch\n";
        return 1;
      }
      for(int i=0;i<map.y;i++) {
        std::vector<json11::Json> row = gridData[i].array_items();
        if(row.size() < map.x) {
          std::cerr << "Error: Solidity layer data size mismatch\n";
          return 1;
        }
        for(int j=0;j<map.x;j++) {
          std::string p = row[j].string_value();
          if(!isBool(p)) {
            std::cerr << "Error: invalid data in solidity field\n";
          }
          bool v = static_cast<bool>(std::stoi(p));
          mapBase[j][i].setSolid(d, v);
        }
      }

    }
  }



  tilesize = tileSize;
  return 0;
}

int Field::loadLevel(const std::string& levelname) {
  std::string complevel = "assets/level/" + levelname + ".sel";
  std::ifstream load(complevel);
  if(!load.is_open())
    return -2;
  std::string line;
  int section = 0;
  unsigned column = 0;
  unsigned row = 0;
  while(load.peek() != EOF) {
    std::getline(load, line);
    //parse line
    if(line.size() == 0 || (line.size() > 0 && line[0] == '#'))
      continue;
    if(section == 0) {
      if(isNum(line)) {
        if((std::stoi(line)-2)%(WINDOW_WIDTH-2) != 0) {
          std::cerr << "Error: invalid width ("<<line<<")\n";
          return -1;
        }
        mapBase.resize(std::stoul(line));
        section++;
      }
      else {
        //std::cerr << line << '\n';
        return -1;  //invalid level: non-integer found in integer parameter;
      }
    }
    else if(section == 1) {
      if(isNum(line)) {
        if((std::stoi(line)-2)%(WINDOW_HEIGHT-2) != 0) {
          std::cerr << "Error: invalid height ("<<line<<")\n";
          return -1;
        }
        for(auto& m : mapBase) {
          m.resize(std::stoul(line));
        }
        section++;
      }
      else {
        return -1; //invalid level: non-integer found in integer parameter;
      }
    }
    else if(section == 2) {
      if(isNum(line)) {
        tilesize.x = std::stoi(line);
        section++;
      }
      else {
        return -1; //invalid level: non-integer found in integer parameter;
      }
    }
    else if(section == 3) {
      if(isNum(line)) {
        tilesize.y = std::stoi(line);
        section++;
      }
      else {
        return -1; //invalid level: non-integer found in integer parameter;
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
  return 0;
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
  sf::Vector2i tile;
  int tileset;
  for(auto x : line) {
    if(x == '`') {
      //add accum to appropriate field
      switch(field) {
        case 0:
          if(!isNum(accum)) {
            return false;
          }
          else {
            tileset = std::stoul(accum);
          }
          break;
        case 1:
          if(!isNum(accum)) {
            return false;
          }
          else {
            tile.x = std::stoul(accum);
          }
          break;
        case 2:
          if(!isNum(accum)) {
            return false;
          }
          else {
            tile.y = std::stoul(accum);
          }
          break;
        case 3:
          if(!isNum(accum) || std::stoi(accum) < 0 || std::stoi(accum) > 15) {
            return false;
          }
          node.setSolid(Up, std::stoi(accum) & 1);
          node.setSolid(Right, !!((std::stoi(accum) & 2) >> 1));
          node.setSolid(Down, !!((std::stoi(accum) & 4) >> 2));
          node.setSolid(Left, !!((std::stoi(accum) & 8) >> 3));
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
  node.setTile(tile);
  node.setTileset(tileset);
  return true;
}
