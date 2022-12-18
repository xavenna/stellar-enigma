#include <iostream>
#include <vector>
#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "stellar-enigma.hpp"

//This is the main file
//Certain customizations need to be done here
int main() {
  //initialization

  //construct MapData object
  MapData mapData(15, unsigned(0.082126 * 288), 55, 20, 20);


  //generate the interface
  int panOfX;
  int msgOfY;
  sf::Vector2i windowSize;
  InterfaceManager interfaceManager;
  windowSize = interfaceManager.initializeInterface(sf::Vector2i(288, 288), msgOfY, panOfX, &mapData.player);


  std::string windowName;  //this is the name of the window
  int frameRate;   //the framerate
  initialSetup(windowName, frameRate);


  sf::RenderWindow window(sf::VideoMode(windowSize.x,windowSize.y), windowName);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(frameRate);


  //setup other useful things
  sf::Font courier;  //font maybe should go somewhere?
  courier.loadFromFile("assets/cour.ttf");

  TextureCache textureCache("assets/texturemap/default.tm");
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


  //also, encapsulate this within Message, maybe in the constructor
  mapData.message.setFont(courier);
  mapData.message.setPosition(40, msgOfY);


  //call custom initialization behavior
  mapData.customInit();

  //more player initialization
  mapData.player.update(mapData.levelSlot.getTilesize());

  //main loop
  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      switch(event.type) {
      case sf::Event::Closed:
	window.close();
	break;
      case sf::Event::KeyPressed:
	// I changed how this works, so this is empty now
	break;
      default:
	break;
      }
    }
    if(!window.isOpen())
      return 0;

    //check if every key is pressed. If key is pressed, call ModeSwitcher::addKey()

    mapData.modeSwitcher.updateKeysPressed();
    int currentMode = mapData.modeSwitcher.getMode();
    //mode-non-specific
    mapData.musicPlayer.assignSounds();
    mapData.musicPlayer.manageSounds();
    
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
      //handle key inputs:
      mapData.event1Handle();
      //other things
      
    }
    else if(currentMode == 2) {
      //cutscene player logic
      mapData.event2Handle();
    }

    interfaceManager.updateInterface(&mapData.player);
    //final drawing
    window.clear();
    //draw based on which mode the engine is currently in
    switch(mapData.modeSwitcher.getMode()) {
    case 0:
      //main menu
      window.draw(mapData.mainMenu.splash);
      break;
    case 1:
    case 2:  //cutscenes work the same as normal
      // eventually find a way to implement animated textures
      //assign sprites
      if(mapData.levelSlot.displayUpdate) {
	mapData.levelSlot.newReadyWindow(mapData.player.getXScreen(), mapData.player.getYScreen());
      }
      for(int i=0;i<WINDOW_WIDTH;i++) {
	for(int j=0;j<WINDOW_HEIGHT;j++) {
	  mapData.levelSlot.assignTextureToWinNode(sf::Vector2i(i,j), textureCache);
	  window.draw(mapData.levelSlot.window[i][j]);
	}
      }

      for(int i=0;i<mapData.levelSlot.getObjNum();i++) {
	//write this
	mapData.levelSlot.assignTextureToObject(i, textureCache);
	if(mapData.levelSlot.displayObject(i, mapData.player.getPos(), mapData.player.getSize())) {
	  window.draw(mapData.levelSlot.getObj(i));
	}
      }



      //update things
      mapData.player.update(mapData.levelSlot.getTilesize());
      //this needs to eventually use player tile size
      window.draw(mapData.player);
      mapData.message.handleMessages();
      mapData.message.wrapMessage();
      window.draw(mapData.message);

      //draw icons
      interfaceManager.drawIcons(window);
      //now, draw border
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
