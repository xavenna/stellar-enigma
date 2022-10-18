#include "player.h"
#include "level.h"

void Player::initialize() {
  xpos = 0;
  ypos = 0;
  facingDir = Up;
  picture.loadFromFile("assets/texture/player.png");
  area.setTexture(picture);
}
void Player::update(sf::Vector2i tilesize) {
  sf::Vector2i mid(xpos+width/2, ypos+height/2);
  xScreen = (mid.x-tilesize.x) / (tilesize.x*(WINDOW_WIDTH-2));
  yScreen = (mid.y-tilesize.y) / (tilesize.y*(WINDOW_HEIGHT-2));
  area.setPosition((mid.x-tilesize.x)%(tilesize.x*(WINDOW_WIDTH-2))+tilesize.x*2-(width/2), (mid.y-tilesize.y)%(tilesize.y*(WINDOW_HEIGHT-2))+tilesize.y*2-(height/2));
}
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
  int temp = int((xpos+(tileWidth / 2)) / tileWidth);
  return temp;
}
int Player::getLevelYPos(int tileHeight) {
  int temp = int((ypos+(tileHeight / 2)) / tileHeight);
  return temp;
}

Player::Player() {

}
