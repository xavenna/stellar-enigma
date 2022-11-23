#include "player.h"
#include "level.h"

void Player::initialize() {
  setPos(0,0);
  facingDir = Up;
  picture.loadFromFile("assets/texture/player.png");
  area.setTexture(picture);
}
void Player::update(sf::Vector2i tilesize) {
  lastPos = pos;
  sf::Vector2i mid(pos.x+size.x/2, pos.y+size.y/2);
  screen.x = (mid.x-tilesize.x) / (tilesize.x*(WINDOW_WIDTH-2));
  screen.y = (mid.y-tilesize.y) / (tilesize.y*(WINDOW_HEIGHT-2));
  area.setPosition((mid.x-tilesize.x)%(tilesize.x*(WINDOW_WIDTH-2))+tilesize.x*2-(size.x/2), (mid.y-tilesize.y)%(tilesize.y*(WINDOW_HEIGHT-2))+tilesize.y*2-(size.y/2));
} // I know this is kind of spaghetti, but it works
void Player::setSpeed(int n) {
  speed = n;
}
int Player::getSpeed() const{
  return speed;
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

Player::Player() {

}
