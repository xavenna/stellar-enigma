#include "player.h"
#include "level.h"

void Player::initialize() {
  xpos = 0;
  ypos = 0;
  speed = 18;  //this is arbitrary
  width = 36;
  height = 36;
  facingDir = Up;
  picture.loadFromFile("assets/texture/player.png");
  area.setTexture(picture);
}
void Player::update(int tilesizeX, int tilesizeY) {
  area.setPosition(xpos, ypos);  //this needs to be fixed to be relative to screen
  xScreen = (int(xpos / tilesizeX / WINDOW_WIDTH));
  yScreen = (int(ypos / tilesizeY / WINDOW_HEIGHT));
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
