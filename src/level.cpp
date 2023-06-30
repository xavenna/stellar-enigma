#include <iostream>
#include <tuple>
#include "level.h"
#include "util.h"

Interaction::Interaction(bool b, int i, sf::Vector2i d) : interaction{b}, object{i}, delta{d} {}
Interaction::Interaction() : interaction{false}, object{0}, delta{0,0} {}


NodeBase Level::getNode(const int& x, const int& y) const {
  return field.getNode(x,y);
}

Object Level::getObj(int index) const {
  return objects.getObj(index);
}
Object& Level::getObjRef(unsigned ind) {
  return objects.getObjRef(ind);
}

Object* Level::getObjPtr(unsigned ind) {
  return objects.getObjPtr(ind);
}

void Level::updateObj(unsigned index, const Object& ob) {
  if(index >= objects.size()) {
    throw std::invalid_argument("Level::updateObj() : Invalid index");
  }
  (objects.getObjRef(index)) = ob;
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

void Level::updateNode(const int& x, const int& y, const NodeBase& node) {
  field.updateNode(x, y, node);
}
Level::Level(const size_t& x, const size_t& y) : field{x, y} {
  tilesizeX = 16;  //for now, this is constant, but it may change in the future
  tilesizeY = 16;
  updateWindowPos();
}
Level::Level() : field{1, 1} {
  tilesizeX = 16;
  tilesizeY = 16;
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
//format:
// 1 char: [oe]; Object or Entity
//if_o:  oxpos`ypos`width`height`solid?`id`value`collectable?`text

int Level::loadLevel(const std::string& levelname) {
  // create the levelName
  field.loadLevel(levelname);
  loadMutables(levelname);
  return 0;
}
void Level::loadMutables(const std::string& levelname) {
  std::string complevel = "assets/level/" + levelname + ".sml";  //stellar mutable list
  std::ifstream get(complevel);
  std::string line;
  std::string accum;
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
    if(line[0] == 'o') {  //this needs to be refactored to fit the new system
      //object
      str2obj(line.substr(1), o);
      o.setPosition(o.getPos().x+tilesizeX, o.getPos().y+tilesizeY);
      objects.storeObj(o.getPos(), o.getSize(), o.getId(), o.getValue(), o.getSolid(), o.getText(), o.getArgs(), getType(o.getId()));
    }
    //the others will be written once objects work
    
  }
  return;
}
int Level::getWidth() const {
  return field.getWidth();
}
int Level::getHeight() const {
  return field.getHeight();
}
int Level::getTilesizeX() const {
  return field.getTilesizeX();
}
int Level::getTilesizeY() const {
  return field.getTilesizeY();
}
sf::Vector2i Level::getTilesize() const {
  return field.getTilesize();
}
int Level::getObjNum() const {
  return objects.size();
}

void Level::assignTextureToWinNode(sf::Vector2i pos, TextureCache& cache) {
  //determine what texture to use and which transformations to apply
  if(pos.x > WINDOW_WIDTH || pos.y > WINDOW_HEIGHT) {
    throw std::invalid_argument("Level::assignTextureToWinNode() : Requested position out of bounds");
  }
  
  CacheNodeAttributes cna;
  int s = cache.tileFilenameHash(window[pos.x][pos.y].getId());
  if(s < 0) {
    //error: requested object doesn't exist
    std::clog << "Texture not assigned to tile ("<<pos.x<<','<<pos.y << ")\n";
    return;
  }
  cna.srcImg = s;
  
  
  //generate cna.tList
  switch(window[pos.x][pos.y].getId()) {
  case 8: {
    //water: add a xShift transformation
    Transform t;
    t.type = Transform::Set_Width;
    t.args[0] = tilesizeX;
    cna.tList.push_back(t);  //set width to one tile
    
    Transform t2;
    t2.type = Transform::Slide_X;
    t2.args[0] = 2*(int(frameCount/10)%8);
    cna.tList.push_back(t2);
  }
    break;
    /*
  case 8: {
    //barrier: add a color Tint
    Transform t;
    t.type = Transform::Tint_Color;
    t.args[0] = (frameCount)%360;
    cna.tList.push_back(t);
    }
    break;
    */
  default:
    //no transformations
    break;
  }
  try {
    //std::cerr << cache.getTexture(cna).getSize().x;
    window[pos.x][pos.y].setTexture(cache.getTexture(cna));
  }
  catch (const std::invalid_argument* e) {
    std::clog << e->what();
  }
  catch (...) {
    std::clog << "Error: target image not found\n";
  }
}

void Level::assignTextureToObject(unsigned index, TextureCache& cache) {
  //determine what texture to use and which transformations to apply
  
  // this needs to use some sort of Object virtual function to take obj_args into account

  if(index > objects.size()) {
    throw std::invalid_argument("Level::assignTextureToObject() : Requested object does not exist");
  }

  Object* ob = objects.getObjPtr(index);
  //create cna using a virtual function
  
  CacheNodeAttributes cna = ob->draw(&cache);
  //to find srcImg, look for objectList[index].getId()
  //int s = cache.objectFilenameHash(objects.getObj(index).getId());
  //if(s < 0) {
    //error: requested object doesn't exist
    //std::cout << "Texture not assigned to object no. "<<index << "\n";
    //return;
  //}
  //cna.srcImg = s;
  
  
  //generate cna.tList
  switch(objects.getObj(index).getId()) {
  default:
    //no transformations
    break;
  }
  try {
    objects.getObjRef(index).setTexture(cache.getTexture(cna));
  }
  catch (...) {
    std::clog << "Error: target image not found\n";
  }
}


int Level::advanceFrameCount() {
  if(frameCount == 1799) {
    frameCount = 0;
    return 0;
  }
  return(++frameCount);
  
}

void Level::addObject(const Object& ob) {
  objects.storeObj(ob, getType(ob.getId()));
}
void Level::removeObject(unsigned index) {
  if(index >= objects.size()) {
    throw std::invalid_argument("Level::removeObject() : Invalid index");
  }
  objects.removeObj(index);
}

void Level::handleObjects(sf::Vector2i pos, sf::Vector2i size) {
  for(unsigned i=0;i<objects.size();i++) {
    Object* x = objects.getObjPtr(i);
    //do things for x.
    x->behave();
    //eventually implement API backend for x->behave();

    
    sf::Vector2i mid(pos.x+size.x/2, pos.y+size.y/2);
    int pscrx = (mid.x-tilesizeX) / (tilesizeX*(WINDOW_WIDTH-2));
    int pscry = (mid.y-tilesizeY) / (tilesizeY*(WINDOW_HEIGHT-2));
    
    sf::Vector2i omid(x->getPos().x+x->getSize().x/2, x->getPos().y+x->getSize().y/2);
    //calculate objects position on player's screen. If it can be displayed, display it:
    
    sf::Vector2i relPos(x->getPos().x-(WINDOW_WIDTH-2)*tilesizeX*pscrx+tilesizeX,
    x->getPos().y-(WINDOW_HEIGHT-2)*tilesizeY*pscry+tilesizeY);
    x->setPosition(relPos.x,relPos.y);
    
    x->setLastPos(x->getPos());
    // If you remove an object, make sure to do i--;
  }
}
bool Level::displayObject(unsigned index, sf::Vector2i ppos, sf::Vector2i size) const {
  
  if(objects.size() <= index) {
    return false;
  }
  
  Object ob{objects.getObj(index)};
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
  
  int pscrx = (mid.x-pxRel)/((WINDOW_WIDTH-2)*field.getTilesize().x);
  int pscry = (mid.y-pyRel)/((WINDOW_HEIGHT-2)*field.getTilesize().y);
  
  //calculate objects position on player's screen. If it can be displayed, display it:
  
  sf::Vector2i relPos(ob.getPos().x-(WINDOW_WIDTH-2)*field.getTilesize().x*pscrx,
              ob.getPos().y-(WINDOW_HEIGHT-2)*field.getTilesize().y*pscry);
  
  if(ob.getId() == 3) {
    //std::clog << relPos.y + ob.getSize().y << "," << field.getTilesize().y << ',' << pscry << '\n';
  }
  if(relPos.x+ob.getSize().x < 0 ||
     relPos.y+ob.getSize().y < 0 ||
     relPos.x + ob.getSize().x >= (WINDOW_WIDTH+2)*field.getTilesize().x ||
     relPos.y + ob.getSize().y >= (WINDOW_HEIGHT+2)*field.getTilesize().y) {
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
  
  
  //check if the resulting move
  //now, iterate through all objects, check if they are solid, and if so,
  //check if they block player's path. If so, shorten move distance.
  
  //make sure to prevent player becoming trapped inside a solid object
  
  sf::Vector2i ts = moveDistance;
  for(int i=0;i<static_cast<int>(objects.size());i++) {
    auto x = objects.getObj(i);
    if(i == ignore) {
      continue;
    }
    if(!x.getSolid()) {
      continue;
    }
    if(speed.y < 0) {
      if(!(pos.x >= x.getPos().x+x.getSize().x || phx <= x.getPos().x) && pos.y >= x.getPos().y+x.getSize().y
       && pos.y-ts.y < x.getPos().y+x.getSize().y) {
    
        ts.y = -(x.getPos().y+x.getSize().y - pos.y);
        fullMove = false;
        moveDistance.y = moveDistance.y > ts.y ? ts.y : moveDistance.y;
      }
    }
    if(speed.x > 0) {
      if(!(pos.y >= x.getPos().y+x.getSize().y || phy <= x.getPos().y) && (phx <= x.getPos().x && phx+ts.x > x.getPos().x)) {
        ts.x = x.getPos().x - phx;  //the math checks out
        fullMove = false;
        moveDistance.x = moveDistance.x > ts.x ? ts.x : moveDistance.x;
      }
    }
    if(speed.y > 0) {
      if(!(pos.x >= x.getPos().x+x.getSize().x || phx <= x.getPos().x) && (phy <= x.getPos().y && phy+tempSpeed > x.getPos().y)) {
        ts.y = x.getPos().y - phy;  //the math checks out
        fullMove = false;
        moveDistance.y = moveDistance.y > ts.x ? ts.x : moveDistance.y;
      }
    }
    if(speed.x < 0) {
      if(!(pos.y >= x.getPos().y+x.getSize().y || phy <= x.getPos().y)
       && pos.x >= x.getPos().x+x.getSize().x
       && pos.x-ts.x < x.getPos().x+x.getSize().x) {
        ts.x = -(x.getPos().x+x.getSize().x - pos.x);
        fullMove = false;
        moveDistance.x = moveDistance.x > ts.x ? ts.x : moveDistance.x;
      }
      break;
    }
  }
  return fullMove ? speed : moveDistance;
}

void Level::handleInteractions() {
  
}

Interaction Level::queryInteractions(const Mutable& mut, int id, int targetId) {
  //get the interaction status of the mutable in question with the map
  //id is the id of mut, to prevent checking self-interactions
  //for mutable not part of Level, id should equal -1
  if(targetId < 0 || unsigned(targetId) > objects.size()) {
    std::clog << "Error: out-of-bounds level mutable interaction status query: id=";
    std::clog << id << ",len=" << objects.size() << "\n";
    return Interaction(false, -1, sf::Vector2i(0,0));
  }

  sf::Vector2i pmin{mut.getPos()};
  sf::Vector2i pmax{pmin+mut.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{mut.getLastPos()};
  sf::Vector2i plmax{plmin+mut.getSize()-sf::Vector2i(1,1)};
  
  sf::Vector2i omin{objects.getObj(targetId).getPos()};
  sf::Vector2i omax{omin+objects.getObj(targetId).getSize() -sf::Vector2i(1,1)};
  sf::Vector2i olmin{objects.getObj(targetId).getLastPos()};
  sf::Vector2i olmax{olmin+objects.getObj(targetId).getSize()-sf::Vector2i(1,1)};


  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  
  sf::Vector2i delta(pmin - plmin); //by default this is the attempted move distance
  if(xInt) {
    if(plmin.x < olmin.x) { //target moved to right
      delta.x = pmax.x - omin.x;
    }
    else { //target moved to left
      delta.x = omax.x - pmin.x;
    }
  }
  if(yInt) {
    if(plmin.y < olmin.y) { //target moved to right
      delta.y = pmax.y - omin.y;
    }
    else { //target moved to left
      delta.y = omax.y - pmin.y;
    }
  }

  //  this will need to be redesigned -- this function can't call behavior functions

  sf::Vector2i maxMovement;
  switch(mut.behaviorType) {
  case 0:
    //solid behave
    break;
  default:
    //no interaction
    break;

  }


  //now, do a dry run of the interaction handler on the object
  //return Interaction(xInt||yInt, targetId, delta > maxMovement ? maxMovement : delta);
  return Interaction(xInt||yInt, targetId, delta);
}


bool str2obj(const std::string& line, Object& obj) {
  //#oxpos`ypos`width`height`solid?`id`value`collectable?`args-csv`text
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
        if(!isNum(accum)) {
          return false;
        }
        else {
          obj.setValue(std::stoi(accum));
        }
        break;
      case 7:
        //parse as a list of 8 comma-delimited integers
        {
          std::vector<std::string> a;
          std::array<int, 8> args;
          parse(accum, a, ",");
          if(a.size() != 8) {
            std::clog << "Error: Wrong number of arguments in mutable list\n";
            return false;
          }
          for(unsigned i=0;i<8;i++) {
            if(!isNum(a[i])) {
              std::clog << "Error: non-number argument in object arguments in mutable list\n";
              return false;
            }
            args[i] = std::stoi(accum);
          }
          
          obj.setArgs(args);
        }
        break;
      case 8:
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
