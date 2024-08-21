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
  switch(config.mode) {
  case Config::FollowPlayerClose:
    return p.getCenter();
    break;
  case Config::FollowPlayerCoarse:
    return p.getCenter();
    break;
  case Config::Fixed:
    return config.focus;
    break;
  }
  
}

float Camera::getScale(const Config& c) {
  switch(config.mode) {
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
    switch(config.mode) {
    case Config::FollowPlayerClose:
      focusPoint = getFocus(config);
      //align camera to sides of map to avoid overhanging
      zoom = config.zoom;
      break;
    case Config::FollowPlayerCoarse:
      zoom = config.zoom;
      break;
    case Config::Fixed:
      focusPoint = config.focus;
      zoom = config.zoom;
      break;
    }
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

Camera::Camera(Player& pl, Level& le, const std::string& fn) : p{pl}, l{le} {
  /*
  //load a list of configs from the specified json file
  std::ifstream read(fn);
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
  //parse json -- extract camera configs
  */
  Config c1;
  c1.mode = Config::Fixed;
  c1.focus = sf::Vector2f(64,96);
  c1.zoom = 0.7;

  Config c2;
  c2.mode = Config::FollowPlayerClose;
  c2.zoom = 2;

  configurations.emplace("fixed", c1);
  configurations.emplace("follow", c2);

  config = c2;

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
