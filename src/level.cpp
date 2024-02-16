#include <iostream>
#include <tuple>
#include "level.h"

Inter::Inter(Object* o, const Player& pl) : p{pl} {
  o1 = o;
  o2 = nullptr;
  player1 = false;
  player2 = true;
  priority = 32;
  subpriority = 32;

}
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

bool Level::notifyObj(msg m) {
  return objects.notify(m);
}

void Level::readyWindow(int xscr, int yscr) {
  //determine which screen to view
  int xwid = WINDOW_WIDTH-2;
  int ywid = WINDOW_HEIGHT-2;
  int xOff = xscr * (xwid);
  int yOff = yscr * (ywid);
  for(unsigned i=0;i<WINDOW_WIDTH;i++) {
    for(unsigned j=0;j<WINDOW_HEIGHT;j++) {
      window[i][j].setId(getNode(static_cast<int>(i)+xOff,static_cast<int>(j)+yOff).getId());
    }
  }
  winOffY = yOff;
  winOffX = xOff;
  displayUpdate = false;
}

void Level::updateNode(int x, int y, const NodeBase& node) {
  field.updateNode(static_cast<unsigned>(x), static_cast<unsigned>(y), node);
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
  if(field.loadLevel(levelname)) {
    return -1;
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
  if(!get.is_open()) {
    return -1;
  }
  while(get.peek() != EOF) {
    std::getline(get, line);
    if(line.size() == 0 || line[0] == '#') {
      continue; //skip line
    }
    //parse `line' as a mutable

    if(!str2obj2(line, o, type)) {
      std::clog << "Error: Failed level load\n";
      return false;
    }
    o.setPosition(static_cast<unsigned>(o.getPos().x)+2*tilesizeX, static_cast<unsigned>(o.getPos().y)+2*tilesizeY);
    if(!objects.storeObj(o, type)) {
      std::clog << "Error: Failed level load\n";
      return false;
    }
    
    
  }
  return true;
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
  sf::Vector2u p{static_cast<unsigned>(pos.x),static_cast<unsigned>(pos.y)};
  CacheNodeAttributes cna;
  cna.srcImg = window[p.x][p.y].getId();
  
  //generate cna.tList
  switch(window[p.x][p.y].getId()) {
  case 8: {
    //water: add a xShift transformation
    Transform t;
    t.type = Transform::Set_Width;
    t.args[0] = static_cast<int>(tilesizeX);
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
    window[p.x][p.y].setTexture(cache.getTexture(cna));
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

Interface Level::handleObjects(sf::Vector2i pos, sf::Vector2i size, SwitchHandler* sh, Utility* u) {
  Interface inter;
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
    inter.openMenu(res.menu); //this means only one menu can be displayed each frame
    for(auto y : res.objs) {
      //create any requested objects
      objects.storeObj(y.first, y.second);
    }
    for(auto y : res.notifications) {
      //send any required messages
      objects.notify(y);
    }
    

    sf::Vector2i mid(pos.x+size.x/2, pos.y+size.y/2);
    unsigned pscrx = (static_cast<unsigned>(mid.x)-tilesizeX) / (tilesizeX*(WINDOW_WIDTH-2));
    unsigned pscry = (static_cast<unsigned>(mid.y)-tilesizeY) / (tilesizeY*(WINDOW_HEIGHT-2));
    
    sf::Vector2i omid(x->getPos().x+x->getSize().x/2, x->getPos().y+x->getSize().y/2);
    //calculate objects position on player's screen. If it can be displayed, display it:
    
    sf::Vector2i relPos(x->getPos().x-static_cast<int>((WINDOW_WIDTH-2)*tilesizeX*pscrx)+static_cast<int>(tilesizeX), x->getPos().y-static_cast<int>((WINDOW_HEIGHT-2)*tilesizeY*pscry)+static_cast<int>(tilesizeY));
    x->setPosition(relPos.x,relPos.y);
    
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
bool Level::displayObject(unsigned index, sf::Vector2i ppos, sf::Vector2i size) const {
  
  if(objects.size() <= index) {
    return false;
  }
  
  Object ob{objects.getObj(index)};

  //don't display invisible objects
  if(ob.Name() == "cutscene_player") {
    return false;
    //add any other invisible objects here
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
    if(phx + speed.x >= getWidth() * getTilesizeX()) { fullMove = false;
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
  bool inString{false};
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
    inString = false;
    std::string name;
    attribs.push_back(ObjAttr());
    x += ','; //this ensures the last argument isn't omitted
    for(auto y : x) {  //for each char of current string...
      if(y == '=') {
        if(inField) {
          std::cerr << "Error: repeat equal in braces\n";
          return false;
        }
        attribs.back().id = accum;
        accum.clear();
        inField = true;
      }
      else if(y == '"') {
        if(inString) {
          inString = false;
          //string is over, parse it
          attribs.back().textArg = accum;
          break;
        }
        else {
          inString = true;
        }
      }
      else if(y == ',') {  // check if it's wrapped in a string
        if(inString) {
          //commas in strings are ignored
          accum += y;
          continue;
        }
        if(accum[0] == '"') { //argument is a string
          if(accum.back() != '"') {
            std::cerr << "Error: unterminated string constant in object list\n";
            std::cerr << "'"<<accum<<"'\n";
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
  obj.setSize(size);
  obj.setText(text);
  obj.resetStatus();
  objType = objClass;
  return true;
}
