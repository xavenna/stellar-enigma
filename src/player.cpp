#include "player.h"
#include "level.h"

Player::Player() {
  setPos(0,0);
  facingDir = Up;
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

//make this match the format of Object::Draw
void Player::assignTexture(TextureCache& cache) {
  CacheNodeAttributes cna;
  int s = cache.reverseHash("star");
  if(s < 0) {
    std::clog << "Error: player sprite not found\n";
    return;
  }
  cna.srcImg = static_cast<unsigned>(s);

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
  if(static_cast<int>(health)+h < 0) {
    health = 0;
  }
  else {
    health = static_cast<unsigned>(static_cast<int>(health) + h);
  }
  return health;
}
unsigned Player::getHealth() const {
  return health;
}
unsigned Player::modifyScore(int s) {
  if(static_cast<int>(score)+s < 0) {
    score = 0;
  }
  else {
    score = static_cast<unsigned>(static_cast<int>(score) + s);
  }
  return score;
}
unsigned Player::getScore() const{
  return score;
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
void Player::setHealth(unsigned n) {
  health = n;
}
void Player::setFacing(Direction n) {
  facingDir = n;
}

void Player::setMaxCooldown(unsigned m) {
  maxCooldown = m;
}
Direction Player::getFacing() const{
  return facingDir;
}
sf::Vector2i Player::getLevelPos(sf::Vector2i tileSize) {
  return sf::Vector2i( ((pos.x+ (tileSize.x / 2)) / tileSize.x), ((pos.y+ (tileSize.y / 2)) / tileSize.y));
}
