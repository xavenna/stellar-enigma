#include "stellar-enigma.hpp"

//This is the main file
int main() {
  //initialization
  //params found in constructor: level to load, texturemap

  //make the constructor less arcane
  MapData mapData(15, unsigned(0.082126 * 288), 55, 20, 20);


  sf::RenderWindow window(sf::VideoMode(static_cast<unsigned>(mapData.windowSize.x), static_cast<unsigned>(mapData.windowSize.y)), "Stellar Enigma Engine Test");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(30);

  //BEGIN Example initialization behavior
  //initialize player
  mapData.player.setPos(32,32);
  mapData.player.setSize(sf::Vector2i(16,16));
  mapData.player.setSpeed(4);
  mapData.player.setHealth(5);
  mapData.player.setMaxCooldown(15);

  //initialize some sound things
  sf::SoundBuffer step;
  step.loadFromFile("assets/audio/thud.wav");
  mapData.musicPlayer.registerSound("step", step);
  mapData.musicPlayer.playMusic("axolotl.ogg");
  //initialize cutscene file

  mapData.modeSwitcher.setMode(0);

  //this could probably be encapsulated into a Menu:: function.
  mapData.mainMenu.baseImage = "blanksplash";
  mapData.mainMenu.tList.clear();
  Transform t;
  t.type = Transform::Add_Text;
  t.args[0] = 8; //change this once the texture base is made
  t.args[1] = 0; //change this once the texture base is made
  t.args[2] = 20; //size, tweak until it looks good
  t.text = "Stellar Enigma Engine Test\n\nVersion v0-unstable\n\n\nCreated by xavenna";
  mapData.mainMenu.tList.push_back(t);
  mapData.mainMenu.onPress = 1;

  // END Example initialization behavior

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
