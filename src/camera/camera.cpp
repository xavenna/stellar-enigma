#include "camera/camera.h"


Config::Config() {

  focus = sf::Vector2f(0,0);
  offset = sf::Vector2f(0,0);
  zoom = 1.f;

  screenSize = sf::Vector2f(96,96);
  scrollDistance = 16.f;
  mode = Config::Fixed;

}

void Animation::reset() {
  usePath = false;
  startPos = {0.f, 0.f};
  startScale = 0.f;
  startAngle = sf::degrees(0.f);

  posStep = {0.f, 0.f};
  scaleStep = 0.f;
  angleStep = sf::degrees(0.f);

  targetPos = {0.f, 0.f};
  targetScale = 0.f;
  targetAngle = sf::degrees(0.f);

  type = Animation::LinSlide;

  endFrame = 0;
  currentFrame = 0;

  nextConfig.clear();

}

float Animation::currentScale(float c) const {
  if(usePath) {
    float t=0;
    switch(type) {
      case Animation::LinSlide:
        return lin_inter(startScale, targetScale, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::LogSlide:
        return log_inter(startScale, targetScale, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::ExpSlide:
        return exp_inter(startScale, targetScale, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::RevExpSlide:
        return revexp_inter(startScale, targetScale, (static_cast<float>(currentFrame)/endFrame));
        break;
      default:
        std::cerr << "Error: Invalid Motion type in animation\n";
        return startScale;
    }

  } else {
    switch(type) {
      case Animation::LinSlide:
        return lin_inter(startScale, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::LogSlide:
        return log_inter(startScale, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::ExpSlide:
        return exp_inter(startScale, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::RevExpSlide:
        return revexp_inter(startScale, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      default:
        return startScale;
    }
  }
}

sf::Angle Animation::currentAngle(sf::Angle c) const {
  if(usePath) {
    float t=0;
    switch(type) {
      case Animation::LinSlide:
        return lin_inter(startAngle, targetAngle, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::LogSlide:
        return log_inter(startAngle, targetAngle, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::ExpSlide:
        return exp_inter(startAngle, targetAngle, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::RevExpSlide:
        return revexp_inter(startAngle, targetAngle, (static_cast<float>(currentFrame)/endFrame));
        break;
      default:
        std::cerr << "Error: Invalid Motion type in animation\n";
        return startAngle;
    }

  } else {
    switch(type) {
      case Animation::LinSlide:
        return lin_inter(startAngle, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::LogSlide:
        return log_inter(startAngle, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::ExpSlide:
        return exp_inter(startAngle, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::RevExpSlide:
        return revexp_inter(startAngle, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      default:
        return startAngle;
    }
  }
}

sf::Vector2f Animation::currentPos(sf::Vector2f c) const {
  if(usePath) {
    float t=0;
    switch(type) {
      case Animation::LinSlide:
        t = static_cast<float>(currentFrame)/endFrame;
        break;
      case Animation::LogSlide:
        t = log_inter(0, 1, static_cast<float>(currentFrame)/endFrame);
        break;
      case Animation::ExpSlide:
        t = exp_inter(0, 1, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::RevExpSlide:
        t = revexp_inter(0, 1, (static_cast<float>(currentFrame)/endFrame));
        break;
      default:
        std::cerr << "Error: Invalid Motion type in animation\n";
        return startPos;

    }
    return path.inter(t);

  } else {
    switch(type) {
      case Animation::LinSlide:
        return lin_inter(startPos, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::LogSlide:
        return log_inter(startPos, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::ExpSlide:
        return exp_inter(startPos, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      case Animation::RevExpSlide:
        return revexp_inter(startPos, c, (static_cast<float>(currentFrame)/endFrame));
        break;
      default:
        std::cerr << "Error: Invalid Motion type in animation\n";
        return startPos;
    }
  }
}

bool Camera::configExists(const std::string& c) {
  return configurations.find(c) != configurations.end();
}

//needs: animType, duration, target config name
void Camera::startAnimation(AnimDesc an) {
  //setup animStatus
  animStatus.reset();
  
  if(an.usePath) {
    if(u.pathList.size() <= an.pathID) {
      std::cerr << "Error: Invalid Path ID "<<an.pathID << "\n";
      return;
    }

    //TODO: PATH ANGLES
    //TODO: Add switching config after a path follow.
    if(an.configName.empty()) {
      //at the end of the animation, snap back to initial config. Don't modify zoom or
      //angle
      //Config conf = configurations[an.configName];
      float z = getScale(config);

      animStatus.startScale = getScale(config);
      if(an.endScale == -1) {
        animStatus.targetScale = getScale(config);
      } else {
        animStatus.targetScale = an.endScale;
      }

      animStatus.startAngle = getAngle(config);
      if(an.endAngle.asDegrees() == -1) {
        animStatus.targetAngle = getAngle(config);
      } else {
        animStatus.targetAngle = an.endAngle;
      }

      animStatus.currentFrame = 0;
      animStatus.endFrame = an.duration;
      animStatus.usePath = true;
      animStatus.path = u.pathList[an.pathID];

      animStatus.type = an.type;
      //since this is a path animation, the position handling is done differently


    } else { //jump to the end config

      Config conf = configurations[an.configName];

      animStatus.startScale = getScale(config);
      animStatus.startAngle = getAngle(config);

      animStatus.targetScale = getScale(conf);
      animStatus.targetAngle = getAngle(conf);

      animStatus.nextConfig = an.configName;

      animStatus.currentFrame = 0;
      animStatus.endFrame = an.duration;
      animStatus.usePath = true;
      animStatus.path = u.pathList[an.pathID];


      config = conf;
      currentConfig = an.configName;

      //at the end of the animation, snap to a new config.
      //Interpolate zoom and angle
    }
    //calculate duration.
    //For position, just interpolate along the path using an appropriate cubic
    inAnim = true;
    return;
  }
  //check if configName is a registered config
  if(configurations.find(an.configName) == configurations.end()) {
    //invalid configuration
    std::cerr << "Error: Invalid config. Exiting\n";
    return;
  }
  else { //check if PATHS are used. If so, work differently.
    if(an.configName == currentConfig) {
      //don't animate if camera doesn't change
      return;
    }
    Config conf = configurations[an.configName];

    float z = getScale(conf);
    sf::Vector2f fp = getFocus(conf);
    sf::Angle ang = getAngle(conf);

    sf::Vector2f fpo = getFocus(config);
    float zo = getScale(config);
    sf::Angle ango = getAngle(config);



    animStatus.startPos = fpo;
    animStatus.startScale = zo;
    animStatus.startAngle = ango;
    animStatus.currentFrame = 0;
    animStatus.endFrame = an.duration;

    animStatus.type = an.type;

    animStatus.usePath = false;

    if(an.type == Animation::LinSlide) {
      animStatus.posStep = (fp - fpo) / static_cast<float>(an.duration);
      animStatus.scaleStep = (z - zo) / static_cast<float>(an.duration);
      animStatus.angleStep = (ang - ango)/static_cast<float>(an.duration);
    }
    else if(an.type == Animation::LogSlide) {


    }
    inAnim = true;
    config = conf;
    currentConfig = an.configName;
  }
  //now, we change the current config so that it is set right for the end of the event

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
    return c.focus;
    break;
  default:
    return zero2<float>();
  }
}

float Camera::getScale(const Config& c) {
  switch(c.mode) {
  case Config::FollowPlayerClose:
    return c.zoom;
    break;
  case Config::FollowPlayerCoarse:
    return c.zoom;
    break;
  case Config::Fixed:
    return c.zoom;
    break;
  default:
    return 1.f;
  }
}

sf::Angle Camera::getAngle(const Config& c) {
  switch(c.mode) {
  case Config::FollowPlayerClose:
    return sf::degrees(c.angle);
    break;
  case Config::FollowPlayerCoarse:
    return sf::degrees(c.angle);
    break;
  case Config::Fixed:
    return sf::degrees(c.angle);
    break;
  default:
    return sf::degrees(0.f);
  }

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


//TODO: Add debug draw mode (renders paths & invisible objects (mode3)
void Camera::gameplayDraw(sf::RenderWindow& window, unsigned mode, TextureCache& cache) {
  //render a frame of regular gameplay
  //First, determine Camera's focus point and zoom scale
  //
  sf::Vector2f focusPoint;
  float zo;
  sf::Angle ang;

  if(inAnim) {
    //an animation is occurring, ignore config settings; use animStatus
    zo = animStatus.currentScale(getScale(config));
    focusPoint = animStatus.currentPos(getFocus(config));
    ang = animStatus.currentAngle(getAngle(config));

    animStatus.currentFrame++;
    if(animStatus.currentFrame == animStatus.endFrame) {
      //animation has concluded
      inAnim = false;
    }
  } else {
    zo = getScale(config);
    focusPoint = getFocus(config);
    ang = getAngle(config);
  }

  //once focus point & zoom have been determined, begin rendering

  sf::FloatRect view;
  sf::Vector2f size; //size of viewing frame
  sf::Vector2f origin; //origin
  size.x = (WINDOW_WIDTH * l.getTilesize().x);
  size.y = (WINDOW_HEIGHT * l.getTilesize().y);

  //ANGLE BREAKS THIS. I think i'll have to do these calculations manually
  //top left of view window
  view.position = (focusPoint - (size / 2.f));

  view.size = size;

  origin = sf::Vector2f(view.position);

  sf::Vector2f offset = static_cast<sf::Vector2f>(l.getTilesize());

  //render tiles:
  for(unsigned i=0;i<l.getWidth();i++) {
    for(unsigned j=0;j<l.getHeight();j++) {
      NodeBase n = l.field.getNode(i,j);
      //apply any transforms to n;
      sf::Vector2f pos = sf::Vector2f(l.getTilesize().x * i, l.getTilesize().y * j); 

      //set texture
      sf::Texture& t = assignTexture(cache, n);
      sf::Sprite s(t);

      s.setPosition(transformAbout(pos, focusPoint, ang, zo) - origin + offset);
      s.setScale({zo, zo});
      s.setRotation(ang);

      sf::FloatRect bnd = s.getGlobalBounds();

      //render m to frame
      if(isInView(bnd)) {
        window.draw(s);
      }
    }
  }


  //render objects

  for(unsigned i=0;i<l.getObjNum();i++) {
    Object& obj = l.getObjRef(i);
    //apply any transforms to n;

    // now, we assign sprites.
    
    //create cna using a virtual function

    CacheNodeAttributes cna = obj.draw();


    //TODO: to allow textures to be repeated instead of scaled up, add an option here.
    // how should an object request a scaled vs tiled texture? A CNA setting?
    // think about this

    try {
      obj.setTexture(cache.getTexture(cna), true);
    }
    catch (...) {
      std::clog << "Error: target image not found\n";
      continue;
    }


    obj.setPosition(transformAbout(obj.getPos(), focusPoint, ang, zo) - origin+offset);
    obj.setScale(zo*obj.getScaleFactor());
    obj.setRotation(ang);

    sf::FloatRect bnd = obj.getGlobalBounds();

    //render m to frame
    if(isInView(bnd)) {
      window.draw(obj);
    }
  }


  //render player

  p.setPosition(transformAbout(p.getPos(), focusPoint, ang, zo)-origin+offset);
  p.setScale({zo, zo});
  p.setRotation(ang);

  sf::FloatRect bnd = p.getGlobalBounds();

  //render m to frame
  if(isInView(bnd)) {
    window.draw(p);
  }
}


//identical to gameplay draw, but with animation capacity
void Camera::cutsceneDraw(sf::RenderWindow& window, unsigned mode, TextureCache& cache) {

  sf::Vector2f focusPoint;
  float zo;
  sf::Angle ang;
  if(inAnim) {
    //an animation is occurring, ignore config settings; use animStatus
    //here, 
    zo = animStatus.currentScale(getScale(config));
    focusPoint = animStatus.currentPos(getFocus(config));
    ang = animStatus.currentAngle(getAngle(config));

    animStatus.currentFrame++;
    if(animStatus.currentFrame == animStatus.endFrame) {
      //animation has concluded
      inAnim = false;
    }
  }
  else {
    focusPoint = getFocus(config);
    zo = getScale(config);
    ang = getAngle(config);
  }


  //once focus point & zoom have been determined, begin rendering
  sf::FloatRect view;
  sf::Vector2f size;
  sf::Vector2f origin;
  size.x = (WINDOW_WIDTH * l.getTilesize().x);
  size.y = (WINDOW_HEIGHT * l.getTilesize().y);
  view.position = (focusPoint - (size / 2.f));
  view.size = size;
  origin = sf::Vector2f(view.position);

  sf::Vector2f offset = static_cast<sf::Vector2f>(l.getTilesize());
  //find 

  //render tiles:
  //i need a null texture.
  //CacheNodeAttributes nullcna;
  //nullcna.srcImg = "null";
  //sf::Sprite s{cache.getTexture(nullcna)};
  for(unsigned i=0;i<l.getWidth();i++) {
    for(unsigned j=0;j<l.getHeight();j++) {
      NodeBase n = l.field.getNode(i,j);
      //apply any transforms to n;
      sf::Vector2f pos = sf::Vector2f(l.getTilesize().x * i, l.getTilesize().y * j); 


      //set texture
      sf::Texture& t = assignTexture(cache, n);
      sf::Sprite s(t);

      s.setPosition(transformAbout(pos, focusPoint, ang, zo)-origin+offset);
      s.setScale({zo, zo});
      s.setRotation(ang);
      sf::FloatRect bnd = s.getGlobalBounds();

      //decide if sprite should be rendered (if it's in the camera's view
      //verify if sprite will intersect with 'view'

      //render m to frame
      if(isInView(bnd)) {
        window.draw(s);
      }

    }


  }

  //render objects

  for(unsigned i=0;i<l.getObjNum();i++) {
    Object& obj = l.getObjRef(i);
    //apply any transforms to n;

    CacheNodeAttributes cna = obj.draw();
    if(cna.srcImg == "\t") {
      std::cerr << '\n';
    }

    try {
      obj.setTexture(cache.getTexture(cna), true);
    }
    catch (...) {
      std::clog << "Error: target image not found\n";
      continue;
    }

    //factor in object's sprite offset

    obj.setPosition(transformAbout(obj.getPos(), focusPoint, ang, zo)-origin+offset);
    obj.setScale(zo*obj.getScaleFactor());
    obj.setRotation(ang);

    sf::FloatRect bnd = obj.getGlobalBounds();
    //render m to frame
    if(isInView(bnd)) {
      window.draw(obj);
    }

  }


  //render player
  p.setPosition(transformAbout(p.getPos(), focusPoint, ang, zo)-origin+offset);
  p.setScale({zo, zo});
  p.setRotation(ang);

  sf::FloatRect bnd = p.getGlobalBounds();
  //render m to frame
  if(isInView(bnd)) {
    window.draw(p);
  }

}
bool Camera::selectConfig(const std::string& c, bool jump) {
  if(configurations.find(c) == configurations.end()) {
    return false;
  }

  if(c != currentConfig && !jump) {
    // if new config is different than last config, then add an animation here
    // Problem: If player moves out of the way during the animation, it breaks.
    AnimDesc a;
    a.duration = 12;
    a.configName = c;
    a.type = Animation::LogSlide;
    a.usePath = false;
    startAnimation(a);
  }

  config = configurations[c];
  currentConfig = c;

  return true;
}

bool Camera::loadConfigs(const std::string& fn) {
  //load a list of configs from the specified json file
  std::ifstream read(fn);
  std::string data;
  if(!read.is_open()) {
    throw std::invalid_argument("Camera::loadConfigs() : Error: Could not open config file");
  }
  getEntireFile(read, data);
  read.close();
  //read contents of save file
  Json::Value k;

  Json::Reader reader;
  reader.parse(data, k);

  k = k["configs"];

  //parse json -- extract camera configs

  for(auto x : k) {
    Config c;
    std::string n;
    if(!generateConfig(x, c, n)) {
      throw std::invalid_argument("Camera::loadConfigs() : Invalid json config ");
    }
    configurations.emplace(n, c);


  }

  config = configurations["follow"];
  return true;
}

Camera::Camera(Player& pl, Level& le, Utility& ut) : p{pl}, l{le}, u{ut} {

}

bool generateConfig(Json::Value ob, Config& c, std::string& name) {
  //ob should be a Json::Value
  //ensure ob has a name and type, and that the type is valid
  Json::Value nameObj = ob["name"];
  if(!nameObj.isString()) {
    std::cerr << "Error: Name is not a string\n";
    return false;
  }
  name = nameObj.asString();
  auto modeObj = ob["mode"];
  if(!modeObj.isString()) {
    std::cerr << "Error: Mode is not a string\n";
    return false;
  }
  if(!isValidConfigType(modeObj.asString())) {
    std::cerr << "Error: Invalid mode\n";
    return false;
  }
  c.mode = mode(modeObj.asString());
  //based on the mode, set all necessary vars
  if(c.mode == Config::Fixed) {
    //should have zoom, focusX, focusY
    auto z = ob["zoom"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.zoom = z.asFloat();
    
    z = ob["focusX"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.focus.x = z.asFloat();

    z = ob["focusY"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.focus.y = z.asFloat();

    z = ob["angle"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.angle = z.asFloat();

  }
  else if(c.mode == Config::FollowPlayerClose) {
    //should have zoom, angle
    auto z = ob["zoom"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.zoom = z.asFloat();

    z = ob["angle"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.angle = z.asFloat();
  }
  else if(c.mode == Config::FollowPlayerCoarse) {
    //should have: zoom, screenSizeXY, offsetXY, angle
    auto z = ob["zoom"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.zoom = z.asFloat();

    z = ob["screenSizeX"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.screenSize.x = z.asFloat();

    z = ob["screenSizeY"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.screenSize.y = z.asFloat();

    z = ob["offsetX"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.offset.x = z.asFloat();

    z = ob["offsetY"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.offset.y = z.asFloat();

    z = ob["angle"];
    if(!z.isNumeric()) {
      std::cerr << "Error: non-numeric argument in numeric field\n";
      return false;
    }
    c.angle = z.asFloat();
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

sf::Texture& assignTexture(TextureCache& cache, NodeBase n) {
  //determine what texture to use and which transformations to apply

  CacheNodeAttributes cna;
  cna.srcImg = "tileset"+std::to_string(n.getTileset());

  //generate cna.tList
  Transform t;
  t.type = Transform::SubRect;
  t.args[0] = n.getTile().x * 16;
  t.args[1] = n.getTile().y * 16;
  t.args[2] = 16;
  t.args[3] = 16;

  cna.tList.push_back(t);

  try {
    sf::Texture& tex = cache.getTexture(cna);
    return tex;
  }
  catch (const std::invalid_argument* e) {
    std::clog << e->what();
  }
  catch (...) {
    std::clog << "Error: target image not found\n";
  }
  cna.srcImg = "null";
  cna.tList.clear();
  return cache.getTexture(cna);
}


bool Camera::isInView(sf::FloatRect obj) const {
  //check if object is contained with view:

  sf::FloatRect view{static_cast<sf::Vector2f>(l.getTilesize()), {WINDOW_WIDTH * static_cast<float>(l.getTilesize().x), WINDOW_HEIGHT * static_cast<float>(l.getTilesize().y)}};

  return (view.findIntersection(obj).has_value());
}


float lin_inter(float origin, float d, float dist) {
  return origin + (d-origin) * dist;
}

//uses smoothstep
float log_inter(float origin, float fin, float dist) {
  if(dist < 0) {
    return origin;
  } else if(dist > 1) {
    return fin;
  }
  return origin + (fin - origin) * (3*dist*dist - 2*dist*dist*dist);

}

float exp_inter(float origin, float fin, float dist) {
  return origin + (fin - origin) * dist * dist;
}

float revexp_inter(float origin, float fin, float dist) {
  return origin - (fin - origin) * (dist * dist - 2*dist);
}


sf::Vector2f lin_inter(sf::Vector2f origin, sf::Vector2f d, float dist) {
  return origin + (d-origin) * dist;
}

sf::Vector2f log_inter(sf::Vector2f origin, sf::Vector2f d, float dist) {
  return sf::Vector2f(log_inter(origin.x, d.x, dist), log_inter(origin.y, d.y, dist));
}

sf::Vector2f exp_inter(sf::Vector2f origin, sf::Vector2f d, float dist) {
  return sf::Vector2f(exp_inter(origin.x, d.x, dist), exp_inter(origin.y, d.y, dist));
}

sf::Vector2f revexp_inter(sf::Vector2f origin, sf::Vector2f d, float dist) {
  return sf::Vector2f(revexp_inter(origin.x, d.x, dist), revexp_inter(origin.y, d.y, dist));
}


sf::Angle lin_inter(sf::Angle origin, sf::Angle d, float dist) {
  return origin + (d-origin) * dist;
}

sf::Angle log_inter(sf::Angle origin, sf::Angle d, float dist) {
  return sf::degrees(log_inter(origin.asDegrees(), d.asDegrees(), dist));
}

sf::Angle exp_inter(sf::Angle origin, sf::Angle d, float dist) {
  return sf::degrees(exp_inter(origin.asDegrees(), d.asDegrees(), dist));
}

sf::Angle revexp_inter(sf::Angle origin, sf::Angle d, float dist) {
  return sf::degrees(revexp_inter(origin.asDegrees(), d.asDegrees(), dist));
}
