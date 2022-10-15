#include "player.h"
#include "level.h"

void Player::initialize() {
  xpos = 0;
  ypos = 0;
  facingDir = Up;
  picture.loadFromFile("assets/texture/player.png");
  area.setTexture(picture);
}
void Player::update() {
  xScreen = (int((xpos-1) / width / (WINDOW_WIDTH-2)));
  yScreen = (int((ypos-1) / height / (WINDOW_HEIGHT-2)));
  area.setPosition((xpos-width)%(width*(WINDOW_WIDTH-2))+width+36, (ypos-height)%(height*(WINDOW_HEIGHT-2))+height+36);
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
