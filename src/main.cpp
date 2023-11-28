#include <iostream>
#include <vector>
#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "stellar-enigma.hpp"

//This is the main file
int main() {
  //initialization

  //construct MapData object
  MapData mapData(15, unsigned(0.082126 * 288), 55, 20, 20);


  try {
    mapData.levelSlot.loadLevel("default");
  }
  catch (int e) {  //make these error codes better
    if(e == 0) {
      std::cout << "Error 0: Level not found\n";
    }
    else if(e == -1) {
      std::cout << "Error -1: Non-integer found in integer parameter\n";
    }
    else if(e == -2) {
      std::cout << "Error: -2\n";
    }
    else if(e == -3) {
      std::cout << "Error: -3\n";
    }
    return e;
  }

  //generate the interface
  int panOfX;
  int msgOfY;
  sf::Vector2i windowSize;
  InterfaceManager interfaceManager;
  windowSize = interfaceManager.initializeInterface(sf::Vector2i(WINDOW_WIDTH * mapData.levelSlot.getTilesize().x, WINDOW_HEIGHT * mapData.levelSlot.getTilesize().y), msgOfY, panOfX, &mapData.player);


  std::string windowName;  //this is the name of the window
  int frameRate;   //the framerate
  initialSetup(windowName, frameRate);


  sf::RenderWindow window(sf::VideoMode(windowSize.x,windowSize.y), windowName);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(frameRate);


  //setup other useful things
  sf::Font courier;  //font maybe should go somewhere, like within Message?
  courier.loadFromFile("assets/cour.ttf");

  TextureCache textureCache("assets/texturemap/default.tm");

  //also, encapsulate this within Message, maybe in the constructor
  mapData.message.setFont(courier);
  mapData.message.setPosition(4+mapData.levelSlot.getTilesize().x, msgOfY);


  //call custom initialization behavior
  mapData.customInit();

  //more player initialization
  mapData.player.update(mapData.levelSlot.getTilesize());
  mapData.player.assignTexture(textureCache);


  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      switch(event.type) {
      case sf::Event::Closed:
	      window.close();
	      break;
      default:
	      break;
      }
    }
    if(!window.isOpen())
      return 0;

    mapData.modeSwitcher.updateKeysPressed();
    int currentMode = mapData.modeSwitcher.getMode();
    

    if(currentMode == 0) {
      //main menu logic
      switch(mapData.event0Handle()) {
      case -1:
	      //kill window
	      window.close();
	      break;
      default:
	      //no special behavior
	      break;
      }
    }
    else if(currentMode == 1) {
      //level player logic
      mapData.event1Handle();
    }
    else if(currentMode == 2) {
      //cutscene player logic
      mapData.event2Handle();
    }
    else if(currentMode == 3) {
      //test mode logic
    }

    //tasks that are common to all modes
    mapData.musicPlayer.assignSounds();
    mapData.musicPlayer.manageSounds();

    interfaceManager.updateInterface(&mapData.player);

    window.clear();
    //draw based on which mode the engine is currently in
    switch(mapData.modeSwitcher.getMode()) {
    case 0:
      //menu mode
      window.draw(mapData.mainMenu.splash);
      break;
    case 1:
    case 2:  //cutscene mode works the same as gameplay mode
    case 3:  //debug mode can use the same drawing logic as normal mode
      //assign sprites
      if(mapData.levelSlot.displayUpdate) {
        mapData.levelSlot.newReadyWindow(mapData.player.getScreen().x, mapData.player.getScreen().y);
      }
      for(int i=0;i<WINDOW_WIDTH;i++) {
        for(int j=0;j<WINDOW_HEIGHT;j++) {
          mapData.levelSlot.assignTextureToWinNode(sf::Vector2i(i,j), textureCache);
          window.draw(mapData.levelSlot.window[i][j]);
        }
      }

      for(int i=0;i<mapData.levelSlot.getObjNum();i++) {
        mapData.levelSlot.assignTextureToObject(i, textureCache);
        if(mapData.levelSlot.displayObject(i, mapData.player.getPos(), mapData.player.getSize())) {
          window.draw(mapData.levelSlot.getObjRef(i));
        }
      }

      //update things
      mapData.player.update(mapData.levelSlot.getTilesize());
      mapData.player.assignTexture(textureCache);
      window.draw(mapData.player);

      //draw icons
      interfaceManager.drawIcons(window);

      //draw message
      mapData.message.handleMessages();
      mapData.message.wrapMessage();
      window.draw(mapData.message);

      //draw border
      for(int i=0;i<interfaceManager.getBorderLen();i++) {
        window.draw(interfaceManager.getBorderElem(i));
      }
      break;
    default:
      //any other modes would go here
      break;
    }
    window.display();
    mapData.levelSlot.advanceFrameCount();
  }
}
