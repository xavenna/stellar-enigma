#include "mutable/player.h"
#include "level/level.h"

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
  sf::Vector2f mid(pos.x+size.x/2, pos.y+size.y/2);
  screen.x = (mid.x-tilesize.x) / (tilesize.x*(WINDOW_WIDTH-2));
  screen.y = (mid.y-tilesize.y) / (tilesize.y*(WINDOW_HEIGHT-2));
  setPosition(
      std::fmod(mid.x-tilesize.x,tilesize.x*(WINDOW_WIDTH-2)) + tilesize.x*2-(size.x/2),
      std::fmod(mid.y-tilesize.y,tilesize.y*(WINDOW_HEIGHT-2))+tilesize.y*2-(size.y/2));

} // I know this is kind of spaghetti, but it works
  // at least, it used to. Now it doesn't and it causes a segfault later
  // :(

//make this match the format of Object::Draw
void Player::assignTexture(TextureCache& cache) {
  CacheNodeAttributes cna;
  cna.srcImg = cache.reverseHash("star");
  //apply transformations
  Transform t;
  t.type = Transform::Rotate;
  int rotationAmount = facingDir;
  t.args[0] = rotationAmount;
  cna.tList.push_back(t);

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
sf::Vector2i Player::getLevelPos(sf::Vector2i tileSize) {
  return sf::Vector2i( ((pos.x+ (tileSize.x / 2)) / tileSize.x), ((pos.y+ (tileSize.y / 2)) / tileSize.y));
}
