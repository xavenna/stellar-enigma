#include "stellar-enigma.hpp"
#include "mapdata.h"

void MapData::customInit() {
  //this function is where any custom code to be run before the main loop should go
  modeSwitcher.setMode(0);
  player.setXPos(36);
  player.setYPos(36);
  player.setWidth(36);
  player.setHeight(36);
  player.setSpeed(18);
  sf::SoundBuffer step;
  step.loadFromFile("assets/audio/thud.wav");
  musicPlayer.registerSound("step", step);
  musicPlayer.playMusic("axolotl.ogg");
  //cutscene
  cutsceneManager.loadCutscenes("default");

  mainMenu.spT.loadFromFile("assets/splash/mainmenu.png");
  mainMenu.splash.setTexture(mainMenu.spT);  //this should likely go somewhere
}

void initialSetup(std::string& name, int& framerate) {
  name = "Stellar Enigma Engine Test";  //this is the name of the window
  framerate = 30;   //the framerate
}

void MapData::event0Handle() {  //this mode is used for the main menu
  sf::Keyboard::Key lk;
  while(modeSwitcher.getLastKey(lk)) {
    if(lk == sf::Keyboard::Enter) {
      modeSwitcher.setMode(1);
    }
  }
}

void MapData::event1Handle() {
  //this is the basic gameplay mode
  int oldX = player.getXPos();
  int oldY = player.getYPos();
  sf::Keyboard::Key lk;
  while(modeSwitcher.getLastKey(lk)) {
    int tempSpeed;
    if(lk == sf::Keyboard::Quote) {
      //query message
      std::cout << message.getMessage() << '\n';
    }
    if(lk == sf::Keyboard::W) {
      player.setFacing(Up);
      tempSpeed = levelSlot.validMove(player);
      player.setYPos(player.getYPos() - tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::A) {
      player.setFacing(Left);
      tempSpeed = levelSlot.validMove(player);
      player.setXPos(player.getXPos() - tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::S) {
      player.setFacing(Down);
      tempSpeed = levelSlot.validMove(player);
      player.setYPos(player.getYPos() + tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::D) {
      player.setFacing(Right);
      tempSpeed = levelSlot.validMove(player);
      player.setXPos(player.getXPos() + tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
  }
  //post-handling thingies

  //check for interaction between player and objects/entities

  //for each element of the object/entity vectors, check if it intersects with the player
  //If so, do something...maybe run a callback from the object/entity
  for(int i=0;i<levelSlot.getObjNum();i++) {
    //check for collision between player and levelSlot.getObject(i);
    Object ob{levelSlot.getObj(i)};
    sf::Vector2i pmin{player.getPos()};
    sf::Vector2i pmax{pmin+player.getSize()-sf::Vector2i(1,1)};

    sf::Vector2i omin{ob.getPos()};
    sf::Vector2i omax{omin+ob.getSize()-sf::Vector2i(1,1)};

    if(pmin.x > omax.x || omin.x > pmax.x || pmin.y > omax.y || omin.y > pmax.y) {
      //no interaction
    }
    else {
      //interaction happens
      //what type of interaction is it?
      //use object id for this
      switch(ob.getId()) {
      case 0:
	//stone: no interaction
	break;
      case 1:
	//crate: attempts to move playerSpeed units in player facing direction

	break;
      case 2:
	//key: is picked up, and something happens. Attempts to play the cutscene specified
	//in ob.text. If a valid cutscene is not specified, play cutscene `key'

	if(cutsceneManager.cutsceneExists(ob.getText())) {
	  cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(ob.getText()));
	}
	else {
	  cutscenePlayer.loadCutscene(cutsceneManager.getCutscene("key"));
	}
	modeSwitcher.setMode(2);
	//destroy object, decrement i;
	levelSlot.removeObject(i);
	i--;
	break;
      case 3:
	//board
	//play the message found in text
	if(ob.getText() != "") {
	  message.addMessage(ob.getText());
	}
	break;
      default:
	//No interaction
	break;
      }
    }
  }

  for(int i=0;i<levelSlot.getEntNum();i++) {
    
  }

  player.update();
  int xmod = WINDOW_WIDTH - 2;
  int ymod = WINDOW_HEIGHT - 2;
  int olpx = oldX / levelSlot.getTilesizeX();
  int olpy = oldY / levelSlot.getTilesizeY();
  int lpx = player.getXPos() / levelSlot.getTilesizeX();
  int lpy = player.getYPos() / levelSlot.getTilesizeY();
  if((olpx % xmod == 0 && lpx == olpx+1) ||
     (lpx % xmod == 0 && olpx == lpx+1) ||
     (olpy % ymod == 0 && lpy == olpy+1) ||
     (lpy % ymod == 0 && olpy == lpy+1)) {

    levelSlot.displayUpdate = true;
  }

  levelSlot.handleEntities();
  levelSlot.handleObjects();

  std::string cn = levelSlot.getNode(player.getLevelXPos(levelSlot.getTilesizeX()), player.getLevelYPos(levelSlot.getTilesizeY())).getCutname();
  if(cn != "") {
    cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(cn));
    modeSwitcher.setMode(2);
  }
}


void MapData::event2Handle() {  //this is the cutscene mode
  int oldX = player.getXPos();
  int oldY = player.getYPos();
  if(cutscenePlayer.updateCutscene(player, message, levelSlot, modeSwitcher, musicPlayer, cutsceneManager)) {
    //I'm not sure if anything needs to go here
  }
  else {
    //cutscene is over; do things now... 
    modeSwitcher.setMode(1); //switch back to gameplay mode
  }
  player.update();
  int xmod = WINDOW_WIDTH - 2;
  int ymod = WINDOW_HEIGHT - 2;
  int olpx = oldX / levelSlot.getTilesizeX();
  int olpy = oldY / levelSlot.getTilesizeY();
  int lpx = player.getXPos() / levelSlot.getTilesizeX();
  int lpy = player.getYPos() / levelSlot.getTilesizeY();
  if((olpx % xmod == 0 && lpx == olpx+1) ||
     (lpx % xmod == 0 && olpx == lpx+1) ||
     (olpy % ymod == 0 && lpy == olpy+1) ||
     (lpy % ymod == 0 && olpy == lpy+1)) {

    levelSlot.displayUpdate = true;
  }
}
