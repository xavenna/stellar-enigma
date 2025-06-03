#include "mapdata/mode.h"
#include <iostream>

//this is for the mode switcher, one of the crucial components of stellar enigma

int ModeSwitcher::getMode() {
  return mode;
}
void ModeSwitcher::setMode(int n) {
  nextMode = n;
}

void ModeSwitcher::updateMode() {
  mode = nextMode;
}

void ModeSwitcher::addKey(sf::Keyboard::Key key) {
  keyList.push_back(key);
}
sf::Keyboard::Key ModeSwitcher::getLastKey() {
  sf::Keyboard::Key e;
  e = keyList.front();
  keyList.pop_front();
  return e;
}
bool ModeSwitcher::getLastKey(sf::Keyboard::Key& sk) {
  if(keyList.size() == 0)
    return false;
  sk = keyList.front();
  keyList.pop_front();
  return true;
}
void ModeSwitcher::ignoreKeypresses() {
  keyList.clear();
}
void ModeSwitcher::cooldown(unsigned i) {
  if(i > 60) { //prevent from cooling down for too long
    i = 60;
  }
  cool = i;
}

ModeSwitcher::ModeSwitcher() {
  mode = 0;
  nextMode = 0;
}


void ModeSwitcher::updateKeysPressed() {
  if(cool != 0) {
    cool--;
    return;
  }
  for(int i=0;i<sf::Keyboard::KeyCount;i++) {
    if(sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))) {
      addKey(static_cast<sf::Keyboard::Key>(i));
    }
  }
}
