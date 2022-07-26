#include "player.h"
#include <iostream>

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
void Player::update() {
  area.setPosition(xpos, ypos);
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
