#include <iostream>
#include <tuple>
#include "level.h"
#include "util.h"

Inter::Inter(Object* o, const Player& pl) : p{pl} {
  o1 = o;
  o2 = nullptr;
  player1 = false;
  player2 = true;
  priority = 32;
  subpriority = 32;

}
Inter::Inter(Object* m, Object* n) : p{15} {
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


NodeBase Level::getNode(const int& x, const int& y) const {
  return field.getNode(x,y);
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

void Level::newReadyWindow(int xscr, int yscr) {
  //determine which screen to view
  int xwid = WINDOW_WIDTH-2;
  int ywid = WINDOW_HEIGHT-2;
  int xOff = xscr * (xwid);
  int yOff = yscr * (ywid);
  for(unsigned i=0;i<WINDOW_WIDTH;i++) {
    for(unsigned j=0;j<WINDOW_HEIGHT;j++) {
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
  for(unsigned i=0;i<WINDOW_WIDTH;i++) {
    for(unsigned j=0;j<WINDOW_HEIGHT;j++) {
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
  std::string type;  //Type of requested object
  if(!get.is_open()) {
    throw 0;
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line.size() == 0 || line[0] == '#') {
      continue; //skip line
    }
    //parse `line' as a mutable

    if(!str2obj2(line, o, type)) {
      std::clog << "Error: Failed level load\n";
      return;
    }
    if(!o.verify()) {
      std::clog << "Error: Invalid object configuration in object of type '";
      std::clog << o.Name() << "'. Object wasn't added\n";
      continue;
      //object isn't added
    }
    else {
      o.setPosition(o.getPos().x+2*tilesizeX, o.getPos().y+2*tilesizeY);
      objects.storeObj(o, type);
    }
    
    
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
unsigned Level::getObjNum() const {
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
    objects.getObjPtr(index)->setTexture(cache.getTexture(cna));
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

void Level::addObject(const Object& ob, const std::string& s) {
  objects.storeObj(ob, s);
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

void Level::handleObjects(sf::Vector2i pos, sf::Vector2i size, SwitchHandler* sh, Message* m) {
  for(unsigned i=0;i<objects.size();i++) {
    Object* x = objects.getObjPtr(i);
    //do things for x.
    Interface res = x->behave(sh);

    if(res.message != "") {
      m->addMessage(res.message);
    }
    /* This function currently has no link to the cutscene machinery
     * This needs to be established eventually.
    if(res.cutscene != "") {
      if(cutsceneManager.cutsceneExists(res.cutscene)) {
        cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(res.cutscene));
      }
      modeSwitcher.setMode(2);
    }
    */

    for(auto y : res.objs) {
      //create any requested objects
      objects.storeObj(y.first, y.second);
    }

    for(auto y : res.notifications) {
      //send any required messages
      objects.notify(y);
    }

    
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
  
  //I'm pretty sure this code is vestigial, as the interaction handler supercedes this.
  /*
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
  */
  return fullMove ? speed : moveDistance;
}

bool str2obj2(const std::string& line, Object& obj, std::string& objType) {
  //parse line. Break into bracketed sections, and interpret each as a vect of strings
  //check if each group has a valid identifier. If so, check if argument list is valid
  //If so, write valus to variables.
  //Otherwise, fail
  std::vector<std::string> ids;
  std::vector<ObjAttr> attribs;
  bool inField{false};
  std::string accum;
  for(auto x : line) {  //parse line into bracketed definitions. IDs and Attribs contain the definitions
    if(x == '{') {
      if(inField) {
        std::clog << "Error: malformed object list : nested braces\n";
        return false;
      }
      inField = true;
    }
    else if(x == '}') {
      if(!inField) {
        std::clog << "Error: malformed object list : close brace outside of braces\n";
        return false;
      }
      inField = false;
      ids.push_back(accum);
      accum.clear();
    }
    else {
      if(!inField) {
        //ignore whitespace
        if(!isEmpty(x)) {
          std::clog << "Error: malformed object list : non-whitespace '"<<x<<"' outside of braces\n";
          return false;
        }
      }
      else {
        accum += x;
      }
    }
  }
  //process each string to extract information, check for errors
  for(auto x : ids) { //transform the bracketed string into an ObjAttr struct
                      //for each string in ids
    accum.clear();
    inField = false;
    std::string name;
    attribs.push_back(ObjAttr());
    x += ','; //this ensures the last argument isn't omitted
    for(auto y : x) {  //for each char of current string...
      if(y == '=') {
        if(inField) {
          std::cerr << "Error: repeat colon in braces\n";
          return false;
        }
        attribs.back().id = accum;
        accum.clear();
        inField = true;
      }
      else if(y == ',') {  // check if it's wrapped in a string
        if(!inField) {
          //text field
          //the entire field, except the final char, is placed in id
          attribs.back().id = x.substr(0, x.size()-1);
          break;
        }
        if(accum[0] == '"') { //argument is a string
          if(accum.back() != '"') {
            std::cerr << "Error: unterminated string constant in object list\n";
            return false;
          }
          accum=accum.substr(1);
          attribs.back().textArg = accum.substr(0,accum.size()-1);

        }
        else if(!isNum(accum)) {
          std::cerr << "Error: non-numeric arg '"<<accum<<"' in field\n";
          return false;
        }
        else {
          attribs.back().args.push_back(std::stoi(accum));
        }
        accum.clear();
      }
      else {
        accum += y;
      }
    }
  }
  //now that data is extracted from bracketed parts, check for validity
  //if it's valid, assign the appropriate attribute
  //TODO: add a way to avoid duplicates
  std::array<int, 8> args;
  std::array<int, 8> switches;
  sf::Vector2i pos{0,0};
  sf::Vector2i size{0,0};
  std::string text;
  std::string objClass;
  int type=-1;
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
      int i=0;
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
      int i=0;
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
    else if(x.id == "t") {
      if(argNum != 1) {
        std::clog << "Error: t field requires 1 argument, "<<argNum<<" provided\n";
        return false;
      }
      //select object type
      type = x.args[0];
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
    else {
      //invalid, or text
      if(x.args.size() == 0) {
        text = x.id;
      }
      else {
        std::clog << "Invalid Identifier '"<<x.id<<"' in object list\n";
        return false;
      }
    }
  }
  //now that values have been extracted, assign values to object
  obj.setLinkID(link);
  obj.setParentID(parent);
  obj.setTextureID(tex);
  obj.setId(type);
  obj.setArgs(args);
  obj.setSwitches(switches);
  obj.setPos(pos);
  obj.setSize(size);
  obj.setText(text);
  objType = objClass;
  return true;
}

