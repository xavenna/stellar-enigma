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
  Message message(34, 55);
  MapData mapData(&player, &modeSwitcher, &levelSlot, &mainMenu, &musicPlayer, &message);

  mainMenu.spT.loadFromFile("assets/splash/mainmenu.png");
  mainMenu.splash.setTexture(mainMenu.spT);

  sf::Font courier;
  courier.loadFromFile("assets/cour.ttf");

  TextureMap textureMap;
  if(!textureMap.initialize("assets/texturemap/default.tm"))  //default texture map, stored in a file
    std::cout << "Error: texture map not found\n";
  try {
    levelSlot.loadLevel("default");  // sel stands for `Stellar Enigma Level'
  }
  catch (int e) {
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

  message.text.setPosition(4, levelSlot.getTilesizeY()*levelSlot.getHeight());
  //this will need to change if I adopt an explor-like scrolling mechanism
  message.text.setCharacterSize(20);
  message.text.setFont(courier);
  message.text.setFillColor(sf::Color::White);

  customInit(mapData);

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
      event0Handle(mapData);
    }
    else if(currentMode == 1) {
      //level player logic
      //handle key inputs:
      event1Handle(mapData);
      //other things
      
    }
    else {
      //invalid mode
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
      //assign sprites
      for(int i=0;i<levelSlot.getWidth();i++) {
	for(int j=0;j<levelSlot.getHeight();j++) {
	  levelSlot.assignTextureToNode(i, j, textureMap);
	  window.draw(levelSlot.getNode(i,j).area);
	}
      }
      //window.draw(message.text);
      //update things
      player.update();
      window.draw(player.area);
      break;
    default:
      //any custom modes would go here
      break;
    }
    window.display();
    //cleanup for next iteration
  }
  //cleanup or smthng
}
