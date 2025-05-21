#include "stellar-enigma.hpp"

//This is the main file
int main() {
  //initialization
  //params found in constructor: level to load, texturemap

  //make the constructor less arcane
  MapData mapData(unsigned(0.082126 * 16 * WINDOW_WIDTH), 55, 20, 20);


  sf::RenderWindow window(sf::VideoMode(static_cast<unsigned>(mapData.windowSize.x), static_cast<unsigned>(mapData.windowSize.y)), "Stellar Enigma Engine Test");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(FRAMERATE);

  //BEGIN Example initialization behavior
  //initialize player
  //player pos should be determined by level file
  mapData.player.setPos(32,32);
  mapData.player.setScaleFactor(sf::Vector2f(1,1));
  mapData.player.setSize(sf::Vector2f(16,16));
  mapData.player.setSpeed(4);
  mapData.player.setHealth(5);
  mapData.player.setMaxCooldown(15);
  mapData.player.setMaxActCooldown(15);

  //initialize some sound things
  mapData.musicPlayer.playMusic("untitled-song-6.ogg");

  mapData.modeSwitcher.setMode(0);

  mapData.mainMenu.loadTemplate("main");


  // END Example initialization behavior

  while(window.isOpen()) { //gameplay loop
    mapData.pollEvents(window);
    if(!window.isOpen())
      break;

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
  //final cleanup
}
