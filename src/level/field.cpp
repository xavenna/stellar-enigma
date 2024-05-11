#include "level/field.h"

Wall::Wall(sf::Vector2f m, sf::Vector2f n) {
  p1 = m;
  p2 = n;
}

sf::Vector2f Wall::push() const {
  //find a normalized vector normal to face vector
  return norm(sf::Vector2f(-face().y, face().x));
}

sf::Vector2f Wall::face() const {
  return p2-p1;
}

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

sf::Vector2f Field::wallHandle(const sf::Vector2f pos, const sf::Vector2f size, const sf::Vector2f speed) const {
  if(magnitude(speed) == 0) {
    return zero2<float>();
  }

  //speed is r

  // TODO:
  // this works great for diagonal movement. For orthogonal movement, it doesn't detect
  // walls that don't pass the center point
  // to rectify: for horizontal movement, use each corner on face as an anchor, find
  // lower t value

  //determine anchor point
  sf::Vector2f moveDir = vsign(speed); //broadly categorizes the move into diag, ortho
  sf::Vector2f anchor = pos + sf::Vector2f( //finds the anchor based on direction
      ((moveDir.x + 1) / 2) * size.x,
      ((moveDir.y + 1) / 2) * size.y
  );
  
  float lowT=1; //lowest value of t1 constant.
  auto terminalWall = walls.end(); //which wall, if any, is blocking the path.
                                   //if set to end, no block occurred
  for(auto it=walls.begin(); it != walls.end(); it++) {
    auto x = *it;
    //if movement is diagonal:
    if(moveDir.x != 0 && moveDir.y != 0) {
      //like orthogonal, but each face touching facing vertex needs to be checked.
      //find the lowest value of the two faces

      //values that are same for both faces
      sf::Vector2f r = x.face();
      sf::Vector2f r0 = x.p1;

      sf::Vector2f p = x.push();
      sf::Vector2f w2 = speed;

      //horizontal face
      sf::Vector2f w0a; //player corner
      sf::Vector2f w1a; //player face
      if(moveDir.y == 1) {
        w0a = pos + size;  // top right
        w1a = sf::Vector2f(-size.x, 0);
      }
      else {
        w0a = pos;  // bottom left
        w1a = sf::Vector2f(size.x, 0);
      }


      //vertical face
      sf::Vector2f w0b;
      sf::Vector2f w1b;
      if(moveDir.x == -1) {
        w0b = pos + sf::Vector2f(0, size.y); //bottom right corner
        w1b = sf::Vector2f(0, -size.y);
      }
      else if(moveDir.x == 1) {
        w0b = pos + sf::Vector2f(size.x, 0);  // top left
        w1b = sf::Vector2f(0, size.y);
      }

      //get lowest t value from each face, then take the minimum
      float t2a = findClosestPointInRange(w0a, w1a, w2, r0, r, p);
      float t2b = findClosestPointInRange(w0b, w1b, w2, r0, r, p);

      float t2 = std::min(t2a, t2b);

      if(lowT > t2) {
        lowT = t2;
        terminalWall = it;
      }
      if(t2 == 0) {
        //if a wall entirely blocks motion, there's no need to iterate over others
        break;
      }
    }
    else {
      //orthogonal movement

      //find values for the specified wall
      sf::Vector2f r = x.face();
      sf::Vector2f r0 = x.p1;

      sf::Vector2f p = x.push();

      sf::Vector2f w0; //player corner
      sf::Vector2f w1; //player face
      if(moveDir.x == -1) {
        w0 = pos + sf::Vector2f(0, size.y); //bottom right corner
        w1 = sf::Vector2f(0, -size.y);
      }
      else if(moveDir.x == 1) {
        w0 = pos + sf::Vector2f(size.x, 0);  // top left
        w1 = sf::Vector2f(0, size.y);
      }
      else if(moveDir.y == 1) {
        w0 = pos + size;  // top right
        w1 = sf::Vector2f(-size.x, 0);
      }
      else {
        w0 = pos;  // bottom left
        w1 = sf::Vector2f(size.x, 0);
      }
      sf::Vector2f w2 = speed;

      float t2 = findClosestPointInRange(w0, w1, w2, r0, r, p);

      if(lowT > t2) {
        lowT = t2;
        terminalWall = it;
      }
      if(t2 == 0) {
        //if a wall entirely blocks motion, there's no need to iterate over others
        break;
      }


    }
  }
  //lowT * speed is the distance player moves
  // check how much residual speed is left
  if(lowT < 1 && terminalWall != walls.end()) {
    //direct residual speed along terminalWall
    sf::Vector2f residSpeed = (1 - lowT) * speed;
    if(dp(residSpeed, (*terminalWall).face()) != 0) { 
      return wallHandle(pos + lowT*speed, size, proj(residSpeed, (*terminalWall).face()));
    }


  }
  return lowT * speed;
}

float Field::findClosestPointInRange(const sf::Vector2f w0, const sf::Vector2f w1, const sf::Vector2f w2, const sf::Vector2f r0, const sf::Vector2f r, const sf::Vector2f p) const {
  if(dp(w2, p) >= 0) {
    //wall doesn't block player movement
    return 1.f;
  }
  if(det2(-r, w2) == 0) {
    //wall is parallel to movement
    return 1.f;
  }

  //set t2=0, find other constant values

  sf::Vector2f constants1 = systemsolve2(-w2, r, w0 - r0);

  //repeat for t2=1
  //this time, we use w0 + w1 as the origin point

  sf::Vector2f constants2 = systemsolve2(-w2, r, (w0+w1) - r0);

  //we now know t1 and t3 at each edge of the player hitbox.
  //now, we check how large the overlap between t2 and t1 is:
  // (constants#.x is t3, constants#.y is t1)

  if(constants1.y >= 1 || constants2.y <= 0) {
    //no overlap occurs.
    return 1.f;
  }

  //using these values, we can calculate the t2 value for both edges of the wall:
  // t2 = m * t1 + b
  float m = 1 / (constants2.y - constants1.y);
  float b = -constants1.y;
  
  //since the wall is a line, we only need to check the values at the edges of the 
  //overlap. The left edge of the overlap is the rightmost of the two left edges:
  //std::max(0, t2(0))

  //the right edge of the overlap is the leftmost of the right edges:
  //std::min(1, t2(1))

  float left = std::max(0.f, b);
  float right = std::min(1.f, m + b);

  //since we have the left and right values, we check the distance to player at each
  //we know the t2 value, so we use it to change the player's start pos
  //
  sf::Vector2f constantsLeft = systemsolve2(-w2, r, (w0 + w1*left) - r0);
  sf::Vector2f constantsRight = systemsolve2(-w2, r, (w0 + w1*right) - r0);

  //the minimum of the t3 values is the one we go with: (t3 is c...x

  if(constantsLeft.x < 0 || constantsLeft.x > 1) {
    //out of range
    if(constantsRight.x < 0 || constantsRight.x > 1) {
      return 1.f;
    }
    else {
      return constantsRight.x;
    }
  }
  else {
    if(constantsRight.x < 0 || constantsRight.x > 1) {
      //right is out of range
      return constantsLeft.x;
    }
    else {
      //both are in range:
      return std::min(constantsLeft.x, constantsRight.x);
    }
  }
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

bool Field::initializeWalls() {
  //creates the walls vector from tile solidity data


  for(unsigned i=0;i<mapBase.size();i++) {
    bool prevLeftWall = false; //bottom to top
    bool prevRightWall = false; //top to bottom
    int leftWallStartPos = 0;
    int rightWallStartPos = 0;
    for(unsigned j=0;j<mapBase[i].size();j++) {

      bool leftWall = mapBase[i][j].getSolid(Left);
      bool rightWall = mapBase[i][j].getSolid(Right);

      if(prevLeftWall && !leftWall) {
        //left wall ends
        //bottom then top
        walls.push_back(Wall(sf::Vector2f(i, leftWallStartPos), sf::Vector2f(i, j)));
      }
      else if(!prevLeftWall && leftWall) {
        //left wall begins
        leftWallStartPos = j;
      }

      if(prevRightWall && !rightWall) {
        //right wall ends
        //top then bottom
        walls.push_back(Wall(sf::Vector2f(i+1, j), sf::Vector2f(i+1, rightWallStartPos)));
      }
      else if(!prevRightWall && rightWall) {
        //right wall begins
        rightWallStartPos = j;
      }

      prevLeftWall = leftWall;
      prevRightWall = rightWall;

    }
  }

  //search rows for horizontal walls
  if(mapBase.size() != 0) {
    for(unsigned j=0;j<mapBase[0].size();j++) {
      bool prevTopWall = false;
      bool prevBottomWall = false;
      int topWallStartPos = 0;
      int bottomWallStartPos = 0;
      for(unsigned i=0;i<mapBase.size();i++) {
        bool bottomWall = mapBase[i][j].getSolid(Down);
        bool topWall = mapBase[i][j].getSolid(Up);
        if(!prevTopWall && topWall) {
          //a top wall begins
          topWallStartPos = i;
        }
        else if(prevTopWall && !topWall) {
          //a top wall ends
          //left, then right
          walls.push_back(Wall(sf::Vector2f(i, j), sf::Vector2f(topWallStartPos, j)));
        }

        if(!prevBottomWall && bottomWall) {
          //a bottom wall begins
          bottomWallStartPos = i;
        }
        else if(prevBottomWall && !bottomWall) {
          //a bottom wall ends
          //right, then left
          walls.push_back(Wall(sf::Vector2f(bottomWallStartPos,j+1), sf::Vector2f(i,j+1)));
        }

        prevTopWall = topWall;
        prevBottomWall = bottomWall;
      }

    }
  }
  for(auto& x : walls) {
    //scale by the tilesize, as walls are currently in terms of tiles
    x.p1.x *= tilesize.x;
    x.p1.y *= tilesize.y; 

    x.p2.x *= tilesize.x;
    x.p2.y *= tilesize.y;
    /*
    std::cerr << "[(" << x.p1.x << "," << x.p1.y << ") -> (" << x.p2.x << "," << x.p2.y;
    std::cerr << ")]\n";
    */
  }

}


int Field::loadLevel(const std::string& levelname) {
  std::cerr << "The use of sel levels is deprecated. Please migrate to json levels\n";
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
