#include "camera/camera.h"


Config::Config() {

  focus = sf::Vector2f(0,0);
  offset = sf::Vector2f(0,0);
  zoom = 1.f;

  screenSize = sf::Vector2f(96,96);
  scrollDistance = 16.f;
  mode = Config::Fixed;

}

//needs: animType, duration, target config name
void Camera::startAnimation(AnimDesc an) {
  //setup animStatus
  
  //check if configName is a registered config
  if(configurations.find(an.configName) == configurations.end()) {
    //invalid configuration
    std::cerr << "Error: Invalid config. Exiting\n";
    return;
  }
  else {
    Config conf = configurations[an.configName];
    float z = getScale(conf);
    sf::Vector2f fp = getFocus(conf);

    sf::Vector2f fpo = getFocus(conf);
    float zo = getScale(conf);

    animStatus.startPos = fpo;
    animStatus.startScale = zo;
    animStatus.currentFrame = 0;
    animStatus.endFrame = an.duration;

    animStatus.posStep = (fp - fpo) / static_cast<float>(an.duration);
    animStatus.scaleStep = (z - zo) / static_cast<float>(an.duration);

    inAnim = true;
  }

}

//assumes mode 1
sf::Vector2f Camera::getFocus(const Config& c) {
  switch(c.mode) {
  case Config::FollowPlayerClose:
    return p.getCenter();
    break;
  case Config::FollowPlayerCoarse:
    //the logic needs to go here
    {
      sf::Vector2f scr;
      scr.x = std::floor((p.getCenter().x - c.offset.x) / c.screenSize.x);
      scr.y = std::floor((p.getCenter().y - c.offset.y) / c.screenSize.y);
      
      scr.x = scr.x * c.screenSize.y + c.offset.y + c.screenSize.x / 2;
      scr.y = scr.y * c.screenSize.y + c.offset.x + c.screenSize.y / 2;
      return scr;

    }
    break;
  case Config::Fixed:
    return config.focus;
    break;
  }
  return zero2<float>();
}

float Camera::getScale(const Config& c) {
  switch(c.mode) {
  case Config::FollowPlayerClose:
    return config.zoom;
    break;
  case Config::FollowPlayerCoarse:
    return config.zoom;
    break;
  case Config::Fixed:
    return config.zoom;
    break;
  }
  return 0.f;
}

sf::RenderTexture& Camera::drawFrame(sf::RenderWindow& window, unsigned mode, TextureCache& cache) {
  //check if mode is valid
  if(mode == 0) {
    //menu, camera isn't used?
  }
  else if(mode == 1 || mode == 3) {
    //standard gameplay display
    gameplayDraw(window, mode, cache);
    
  }
  else if(mode == 2) {
    //cutscene mode
    cutsceneDraw(window, mode, cache);
  }

  return frame;

}


void Camera::gameplayDraw(sf::RenderWindow& window, unsigned mode, TextureCache& cache) {
  //render a frame of regular gameplay
  //First, determine Camera's focus point and zoom scale
  sf::Vector2f focusPoint = getFocus(config);
  float zoom = getScale(config);


  //once focus point & zoom have been determined, begin rendering
  sf::FloatRect view;
  sf::Vector2f size;
  sf::Vector2f origin;
  size.x = (WINDOW_WIDTH * l.getTilesize().x);
  size.y = (WINDOW_HEIGHT * l.getTilesize().y);
  view.left = (focusPoint - (size / 2.f)).x;
  view.top = (focusPoint - (size / 2.f)).y;
  view.width = size.x;
  view.height = size.y;
  origin = sf::Vector2f(view.left, view.top);


  sf::Vector2f offset = static_cast<sf::Vector2f>(l.getTilesize());

  //render tiles:
  sf::Sprite s;
  for(int i=0;i<l.getWidth();i++) {
    for(int j=0;j<l.getHeight();j++) {
      NodeBase n = l.field.getNode(i,j);
      //apply any transforms to n;
      sf::Vector2f pos = sf::Vector2f(l.getTilesize().x * i, l.getTilesize().y * j); 

      s.setPosition(zoom * (pos - focusPoint) + focusPoint);
      s.setScale(zoom, zoom);

      //set texture
      assignTexture(s, cache, n);
      sf::FloatRect spriteBox = s.getGlobalBounds();

      //render m to frame
      if(view.intersects(spriteBox)) {
        s.setPosition(s.getPosition() - origin + offset);
        window.draw(s);
      }
    }
  }


  //render objects

  for(unsigned i=0;i<l.getObjNum();i++) {
    Object& obj = l.getObjRef(i);
    //apply any transforms to n;

    obj.setPosition(zoom * (obj.getPos() - focusPoint) + focusPoint);
    obj.setScale(zoom, zoom);

    //render m to frame
    if(view.intersects(obj.getGlobalBounds())) {
      obj.setPosition(obj.getPosition() - origin + offset);
      window.draw(obj);
    }
  }


  //render player
  p.setPosition(zoom * (p.getPos() - focusPoint) + focusPoint);
  p.setScale(zoom, zoom);

  //render m to frame
  if(view.intersects(p.getGlobalBounds())) {
    p.setPosition(p.getPosition() - origin + offset);
    window.draw(p);
  }
}


//identical to gameplay draw, but with animation capacity
void Camera::cutsceneDraw(sf::RenderWindow& window, unsigned mode, TextureCache& cache) {

  sf::Vector2f focusPoint;
  float zoom;
  if(inAnim) {
    //an animation is occurring, ignore config settings; use animStatus
    zoom = animStatus.startScale + (animStatus.scaleStep * animStatus.currentFrame);
    focusPoint = animStatus.startPos + (animStatus.posStep * static_cast<float>(animStatus.currentFrame));
    animStatus.currentFrame++;
    if(animStatus.currentFrame == animStatus.endFrame) {
      //animation has concluded
      inAnim = false;
    }
  }
  else {
    focusPoint = getFocus(config);
    zoom = config.zoom;
  }


  //once focus point & zoom have been determined, begin rendering
  sf::FloatRect view;
  sf::Vector2f size;
  sf::Vector2f origin;
  size.x = (WINDOW_WIDTH * l.getTilesize().x);
  size.y = (WINDOW_HEIGHT * l.getTilesize().y);
  view.left = (focusPoint - (size / 2.f)).x;
  view.top = (focusPoint - (size / 2.f)).y;
  view.width = size.x;
  view.height = size.y;
  origin = sf::Vector2f(view.left, view.top);

  sf::Vector2f offset = static_cast<sf::Vector2f>(l.getTilesize());
  //find 

  //render tiles:
  sf::Sprite s;
  for(int i=0;i<l.getWidth();i++) {
    for(int j=0;j<l.getHeight();j++) {
      NodeBase n = l.field.getNode(i,j);
      //apply any transforms to n;
      sf::Vector2f pos = sf::Vector2f(l.getTilesize().x * i, l.getTilesize().y * j); 

      s.setPosition(zoom * (pos - focusPoint) + focusPoint);
      s.setScale(zoom, zoom);

      //set texture
      assignTexture(s, cache, n);
      sf::FloatRect spriteBox = s.getGlobalBounds();
      //decide if sprite should be rendered (if it's in the camera's view
      //verify if sprite will intersect with 'view'

      //render m to frame
      if(view.intersects(spriteBox)) {
        s.setPosition(s.getPosition() - origin + offset);
        window.draw(s);
      }

    }


  }

  //render objects

  for(unsigned i=0;i<l.getObjNum();i++) {
    Object& obj = l.getObjRef(i);
    //apply any transforms to n;

    obj.setPosition(zoom * (obj.getPos() - focusPoint) + focusPoint);
    obj.setScale(zoom, zoom);

    //render m to frame
    if(view.intersects(obj.getGlobalBounds())) {
      obj.setPosition(obj.getPosition() - origin + offset);
      window.draw(obj);
    }

  }

  //render player
  p.setPosition(zoom * (p.getPos() - focusPoint) + focusPoint);
  p.setScale(zoom, zoom);

  //render m to frame
  if(view.intersects(p.getGlobalBounds())) {
    p.setPosition(p.getPosition() - origin + offset);
    window.draw(p);
  }

}
bool Camera::selectConfig(const std::string& c) {
  if(configurations.find(c) == configurations.end()) {
    return false;
  }
  config = configurations[c];
  return true;
}

Camera::Camera(Player& pl, Level& le, const std::string& fn) : p{pl}, l{le} {
  //load a list of configs from the specified json file
  std::ifstream read(fn);
  std::string data;
  std::string err;
  if(!read.is_open()) {
    throw std::invalid_argument("Camera::Camera() : Error: Could not open config file");
  }
  getEntireFile(read, data);
  read.close();
  //read contents of save file
  json11::Json save;
  save = save.parse(data, err);
  if(!err.empty()) {
    throw std::invalid_argument("Camera::Camera() : Json parse error: '"+err);
  }
  //parse json -- extract camera configs

  json11::Json::object k = save.object_items();


  json11::Json::array confs = k["configs"].array_items();

  for(auto x : confs) {
    Config c;
    std::string n;
    if(!generateConfig(x, c, n)) {
      throw std::invalid_argument("Camera::Camera() : Invalid json config ");
    }
    configurations.emplace(n, c);


  }

  config = configurations["follow"];

}

bool generateConfig(json11::Json ob, Config& c, std::string& name) {
  //ob should be a json11::Json::object
  //ensure ob has a name and type, and that the type is valid
  auto nameObj = ob["name"];
  if(!nameObj.is_string()) {
    std::cerr << "Error: Name is not a string\n";
    return false;
  }
  name = nameObj.string_value();
  auto modeObj = ob["mode"];
  if(!modeObj.is_string()) {
    std::cerr << "Error: Mode is not a string\n";
    return false;
  }
  if(!isValidConfigType(modeObj.string_value())) {
    std::cerr << "Error: Invalid mode\n";
    return false;
  }
  c.mode = mode(modeObj.string_value());
  //based on the mode, set all necessary vars
  if(c.mode == Config::Fixed) {
    //should have zoom, focusX, focusY
    auto z = ob["zoom"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.zoom = z.number_value();
    
    z = ob["focusX"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.focus.x = z.number_value();

    z = ob["focusY"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.focus.y = z.number_value();

  }
  else if(c.mode == Config::FollowPlayerClose) {
    //should have zoom
    auto z = ob["zoom"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.zoom = z.number_value();
  }
  else if(c.mode == Config::FollowPlayerCoarse) {
    //should have: zoom, screenSizeXY, offsetXY 
    auto z = ob["zoom"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.zoom = z.number_value();

    z = ob["screenSizeX"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.screenSize.x = z.number_value();

    z = ob["screenSizeY"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.screenSize.y = z.number_value();

    z = ob["offsetX"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.offset.x = z.number_value();

    z = ob["offsetY"];
    if(!z.is_number()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.offset.y = z.number_value();
  }
  return true;
}

bool isValidConfigType(const std::string& n) {
  if(n == "fixed" || n == "followClose" || n == "followCoarse") {
    return true;
  }
  return false;
}

Config::Mode mode(const std::string& n) {
  if(n == "fixed") {
    return Config::Fixed;
  }
  else if(n == "followClose") {
    return Config::FollowPlayerClose;
  }
  else if(n == "followCoarse") {
    return Config::FollowPlayerCoarse;
  }
  throw std::invalid_argument("mode() : Invalid mode '" + n  + "'");
}

void assignTexture(sf::Sprite& s, TextureCache& cache, NodeBase n) {
  //determine what texture to use and which transformations to apply

  CacheNodeAttributes cna;
  cna.srcImg = n.getTileset();

  //generate cna.tList
  Transform t;
  t.type = Transform::SubRect;
  t.args[0] = n.getTile().x * 16;
  t.args[1] = n.getTile().y * 16;
  t.args[2] = 16;
  t.args[3] = 16;

  cna.tList.push_back(t);

  try {
    s.setTexture(cache.getTexture(cna));
  }
  catch (const std::invalid_argument* e) {
    std::clog << e->what();
  }
  catch (...) {
    std::clog << "Error: target image not found\n";
  }
}
