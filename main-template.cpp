#include "stellar-enigma.hpp"

//This is the main file
int main() {
  //initialization

  MapData mapData(15, unsigned(0.082126 * 288), 55, 20, 20);


  sf::RenderWindow window(sf::VideoMode(mapData.windowSize.x, mapData.windowSize.y), "Stellar Enigma Engine Test");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(30);

  //place custom initialization behavior here

  while(window.isOpen()) { //gameplay loop
    mapData.pollEvents(window);
    if(!window.isOpen())
      return 0;

    //handle event and run mode-specific tasks
    switch(mapData.handleEvents()) {
    case -1:
      //kill window
      window.close();
      break;
    default:
      //no behaviors have been requested
      break;
    }

    //draw things, finish up the frame
    mapData.finishFrame(window);
  }
}
