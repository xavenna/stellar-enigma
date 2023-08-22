#include "player.h"
#include "level.h"

Player::Player(unsigned c) {
  setPos(0,0);
  facingDir = Up;
  maxCooldown = c;
}
void Player::update(sf::Vector2i tilesize) {
  //set facing direction;
  if(pos.x < lastPos.x && pos.y == lastPos.y) {
    facingDir = Left;
  }
  else if(pos.x > lastPos.x && pos.y == lastPos.y) {
    facingDir = Right;
  }
  else if(pos.y < lastPos.y && pos.x == lastPos.x) {
    facingDir = Up;
  }
  else if(pos.y > lastPos.y && pos.x == lastPos.x) {
    facingDir = Down;
  }
  lastPos = pos;
  sf::Vector2i mid(pos.x+size.x/2, pos.y+size.y/2);
  screen.x = (mid.x-tilesize.x) / (tilesize.x*(WINDOW_WIDTH-2));
  screen.y = (mid.y-tilesize.y) / (tilesize.y*(WINDOW_HEIGHT-2));
  setPosition((mid.x-tilesize.x)%(tilesize.x*(WINDOW_WIDTH-2))+tilesize.x*2-(size.x/2), (mid.y-tilesize.y)%(tilesize.y*(WINDOW_HEIGHT-2))+tilesize.y*2-(size.y/2));

} // I know this is kind of spaghetti, but it works

void Player::assignTexture(TextureCache& cache) {
  CacheNodeAttributes cna;
  // player sprite is categorized as an entity
  int s = cache.reverseHash("star");
  if(s < 0) {
    std::clog << "Error: player sprite not found\n";
    return;
  }
  cna.srcImg = s;

  //apply transformations
  Transform t;
  t.type = Transform::Rotate;
  t.args[0] = facingDir;
  cna.tList.push_back(t);

  try {
    setTexture(cache.getTexture(cna));
  }
  catch (...) {
    std::clog << "Error: target image not found\n";
  }
}

void Player::clearEffects() {
  effects.clear();
}
void Player::applyEffect(const Effect& e) {
  effects.push_back(e);
}

unsigned Player::decrementCooldown() {
  return cooldown==0?0:(--cooldown);
}
unsigned Player::getCooldown() {
  return cooldown;
}
void Player::resetCooldown() {
  cooldown = maxCooldown;
}
unsigned Player::modifyHealth(int h) {
  return int(health)+h<0?0:(health+=h);
}
unsigned Player::getHealth() {
  return health;
}

void Player::setSpeed(int n) {
  speed = n;
}
int Player::getRawSpeed() const{
  return speed;
}
int Player::getSpeed() const {
  float totalScale{1};
  for(auto x : effects) {
    //check if effect modifies speed
    switch(x.getType()) {
    case Effect::Speed:
      //return modified value
      totalScale*=x.getScale();
      break;
    default:
      continue;
    }
  }
  return speed * totalScale;
}
void Player::setHealth(int n) {
  health = n;
}
int Player::getHealth() const{
  return health;
}
void Player::setFacing(Direction n) {
  facingDir = n;
}
Direction Player::getFacing() const{
  return facingDir;
}
int Player::getLevelXPos(int tileWidth) {
  //player width maybe needs to be substituted for tileWidth
  int temp = int((pos.x+(tileWidth / 2)) / tileWidth);
  return temp;
}
int Player::getLevelYPos(int tileHeight) {
  int temp = int((pos.y+(tileHeight / 2)) / tileHeight);
  return temp;
}
