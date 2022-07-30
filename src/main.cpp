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
  std::string windowName;  //this is the name of the window
  int windowWidth;  //this is the width of the window, in pixels
  int windowHeight;  //this is the height of the window
  int frameRate;   //the framerate
  initialSetup(windowName, windowWidth, windowHeight, frameRate);


  sf::RenderWindow window(sf::VideoMode(windowWidth,windowHeight), windowName);
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(frameRate);

  //modes: 0=main menu, 1=level player, 2=cutscene
  Player player;
  player.initialize();
  ModeSwitcher modeSwitcher;
  Level levelSlot;
  Menu mainMenu;
  MusicPlayer musicPlayer;
  Message message(34, 55, 20);
  CutscenePlayer cutscenePlayer;
  MapData mapData(&player, &modeSwitcher, &levelSlot, &mainMenu, &musicPlayer, &message, &cutscenePlayer);

  mainMenu.spT.loadFromFile("assets/splash/mainmenu.png");
  mainMenu.splash.setTexture(mainMenu.spT);  //this should likely go somewhere

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

  player.update(levelSlot.getTilesizeX(), levelSlot.getTilesizeY());

  message.text.setPosition(4, levelSlot.getTilesizeY()*WINDOW_HEIGHT);

  //also, encapsulate this within Message, maybe in the constructor
  message.text.setCharacterSize(20);
  message.text.setFont(courier);
  message.text.setFillColor(sf::Color::White);
  message.text.setString("");

  mapData.customInit();

  while(window.isOpen()) {
    sf::Event event;
    while(window.pollEvent(event)) {
      switch(event.type) {
      case sf::Event::Closed:
	window.close();
	break;
      case sf::Event::KeyPressed:
	modeSwitcher.addKey(event.key.code);
	if(event.key.code == sf::Keyboard::Enter)
	break;
      default:
	break;
      }
    }
    if(!window.isOpen())
      return 0;
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
	levelSlot.readyWindow(player.getXScreen(), player.getYScreen());
      }
      for(int i=0;i<WINDOW_WIDTH;i++) {
	for(int j=0;j<WINDOW_HEIGHT;j++) {
	  levelSlot.assignTextureToWinNode(i, j, textureMap);
	  window.draw(levelSlot.window[i][j].area);
	}
      }

      //update things
      player.update(levelSlot.getTilesizeX(), levelSlot.getTilesizeY());
      //this needs to eventually use player tile size
      window.draw(player.area);
      message.handleMessages();
      message.wrapMessage();
      window.draw(message.text);
      //this doesn't work
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
