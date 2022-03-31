#include "stellar-enigma.hpp"

//this file contains custom elements.  Data and functions for the game go in this file

void customInit(MapData& md) {
  //custom initialization goes here
  //such as registering sound, initializing player, etc.
}

//an event#Handle function is needed for each mode the game uses
void event0Handle(MapData& md) {
  sf::Keyboard::Key lk;
  while(md.modeSwitcher.getLastKey(lk)) {
    //event handling goes here
  }
  //other processing goes here
}

void initialSetup(std::string& name, int& width, int& height, int& framerate) {
  //window name, width, height, and framerate must be defined here
}
