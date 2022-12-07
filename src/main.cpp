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

  Player player;
  player.initialize();
  ModeSwitcher modeSwitcher;
  Level levelSlot;
  Menu mainMenu;
  MusicPlayer musicPlayer;
  Message message(int(0.082126 * 288), 55, 20); //magic numbers, whooooooo
  CutscenePlayer cutscenePlayer;
  CutsceneManager cutsceneManager;
  MapData mapData(&player, &modeSwitcher, &levelSlot, &mainMenu, &musicPlayer, &message, &cutscenePlayer, &cutsceneManager);


  //this needs to be reordered to suit the new interface manager system
  int panOfX;
  int msgOfY;
  sf::Vector2i windowSize;
  InterfaceManager interfaceManager;
  windowSize = interfaceManager.initializeInterface(sf::Vector2i(288, 288), msgOfY, panOfX, &player);

  std::string windowName;  //this is the name of the window
  int frameRate;   //the framerate
  initialSetup(windowName, frameRate);


  sf::RenderWindow window(sf::VideoMode(windowSize.x,windowSize.y), windowName);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(frameRate);

  //modes: 0=main menu, 1=level player, 2=cutscene


  sf::Font courier;  //font maybe should go somewhere?
  courier.loadFromFile("assets/cour.ttf");

  TextureMap textureMap;
  if(!textureMap.initialize("assets/texturemap/default.tm"))  //default texture map, stored in a file
    std::cout << "Error: texture map not found\n";
  try {
    levelSlot.loadLevel("default");
  }
  catch (int e) {  //make these error codes better
    if(e == 0) {
      std::cout << "Error 0: Level not found\n";
    }
    else if(e == -1) {
      std::cout << "Error: -1\n";
    }
    else if(e == -2) {
      std::cout << "Error: -2\n";
    }
    else if(e == -3) {
      std::cout << "Error: -3\n";
    }
    return e;
  }
  message.text.setPosition(40, msgOfY);


  //also, encapsulate this within Message, maybe in the constructor
  message.text.setCharacterSize(20);
  message.text.setFont(courier);
  message.text.setFillColor(sf::Color::White);
  message.text.setString("");

  mapData.customInit();

  player.update(levelSlot.getTilesize());

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

    modeSwitcher.updateKeysPressed();
    int currentMode = modeSwitcher.getMode();
    //mode-non-specific
    musicPlayer.assignSounds();
    musicPlayer.manageSounds();
    
    if(currentMode == 0) {
      //main menu logic
      mapData.event0Handle();
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


    interfaceManager.updateInterface(&player);
    //final drawing
    window.clear();
    //draw based on which mode the engine is currently in
    switch(modeSwitcher.getMode()) {
    case 0:
      //main menu
      window.draw(mainMenu.splash);
      break;
    case 1:
    case 2:  //cutscenes work the same as normal
      //assign sprites
      if(levelSlot.displayUpdate) {
	levelSlot.newReadyWindow(player.getXScreen(), player.getYScreen());
      }
      for(int i=0;i<WINDOW_WIDTH;i++) {
	for(int j=0;j<WINDOW_HEIGHT;j++) {
	  levelSlot.assignTextureToWinNode(i, j, textureMap);
	  window.draw(levelSlot.window[i][j].area);
	}
      }

      for(int i=0;i<levelSlot.getObjNum();i++) {
	//write this
	levelSlot.assignTextureToObject(i, textureMap);
	if(levelSlot.displayObject(i, player.getPos(), player.getSize())) {
	  window.draw(levelSlot.getObj(i).area);
	}
      }
      //update things
      player.update(levelSlot.getTilesize());
      //this needs to eventually use player tile size
      window.draw(player.area);
      message.handleMessages();
      message.wrapMessage();
      window.draw(message.text);

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
    //cleanup for next iteration
  }
  //cleanup or smthng
}
