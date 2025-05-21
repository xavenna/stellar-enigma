#include "mutable/player.h"
#include "level/level.h"

Player::Player() {
  setPos(0,0);
  facingDir = Up;
}
void Player::update() {
  lastPos = pos;
}
//make this match the format of Object::Draw
void Player::assignTexture(TextureCache& cache) {
  CacheNodeAttributes cna;
  std::string name;
  switch(facingDir) {
  case Up:
    name = "star-b";
    break;
  case Right:
    name = "star-r";
    break;
  case Down:
    name = "star";
    break;
  case Left:
    name = "star-l";
    break;
  default:
    name = "star";
    break;
  }

  cna.srcImg = name;
  //apply transformations

  try {
    setTexture(cache.getTexture(cna));
  }
  catch (...) {
    std::clog << "Error: target image not found\n";
  }
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

unsigned Player::decrementActCooldown() {
  return actCooldown==0?0:(--actCooldown);
}
unsigned Player::getActCooldown() {
  return actCooldown;
}
void Player::resetActCooldown() {
  actCooldown = maxActCooldown;
}
void Player::setMaxActCooldown(unsigned m) {
  maxActCooldown = m;
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

void Player::setSpeed(float n) {
  speed = n;
}
float Player::getRawSpeed() const{
  return speed;
}
float Player::getSpeed() const {
  if(grabbing == true) {
    return speed * 0.5;
  }
  return speed;
}
void Player::setHealth(unsigned n) {
  health = n;
}
void Player::setFacing(Direction n) {
  facingDir = n;
}

Object* Player::getHeldObj() {
  return heldObj;
}
void Player::setHeldObj(Object* o) {
  heldObj = o;
}
void Player::setMaxCooldown(unsigned m) {
  maxCooldown = m;
}
Direction Player::getFacing() const{
  return facingDir;
}

void Player::setSize(sf::Vector2f n) {
  size = n;
}

sf::Vector2f Player::getSize() const {
  return size;
}

sf::Vector2i Player::getLevelPos(sf::Vector2i tileSize) {
  return sf::Vector2i( ((pos.x+ (tileSize.x / 2)) / tileSize.x), ((pos.y+ (tileSize.y / 2)) / tileSize.y));
}
