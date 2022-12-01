#include "stellar-enigma.hpp"
#include "mapdata.h"
#include "callback.h"

void MapData::customInit() {
  //this function is where any custom code to be run before the main loop should go
  modeSwitcher.setMode(0);
  player.setXPos(36);
  player.setYPos(36);
  player.setWidth(36);
  player.setHeight(36);
  player.setSpeed(6);
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

// TODO: modify this function to fix diagonal movement bug MkII 

void MapData::event1Handle() {
  //this is the basic gameplay mode
  sf::Keyboard::Key lk;
  sf::Vector2i moveDir(0,0);
  while(modeSwitcher.getLastKey(lk)) {
    if(lk == sf::Keyboard::Quote) {
      //query message
      std::cout << message.getMessage() << '\n';
    }
    if(lk == sf::Keyboard::W) {
      if(moveDir.y == 0)
	moveDir.y = -1;
      else
	moveDir.y = 0;
    }
    else if(lk == sf::Keyboard::A) {
      if(moveDir.x == 0)
	moveDir.x = -1;
      else
	moveDir.x = 0;
    }
    else if(lk == sf::Keyboard::S) {
      if(moveDir.y == 0)
	moveDir.y = 1;
      else
	moveDir.y = 0;
    }
    else if(lk == sf::Keyboard::D) {
      if(moveDir.x == 0)
	moveDir.x = 1;
      else
	moveDir.x = 0;
    }
  }
  //std::cout << moveDir.x << ',' << moveDir.y << '\n';
  //now that all input has been gathered, handle movement
  int tempSpeed;
  sf::Vector2i sp(moveDir.x * player.getSpeed(), moveDir.y * player.getSpeed());
  /*
  if(moveDir.x == 0) {
    if(moveDir.y == 0) {
      //no motion

    }
    else if(moveDir.y == 1) {
      //move down
      player.setFacing(Down);
      tempSpeed = levelSlot.validMove(player.getPos(), player.getSize(), player.getSpeed(), player.getFacing());
      player.setYPos(player.getYPos() + tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(moveDir.y == -1) {
      //move up
      player.setFacing(Up);
      tempSpeed = levelSlot.validMove(player.getPos(), player.getSize(), player.getSpeed(), player.getFacing());
      player.setYPos(player.getYPos() - tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
  }
  else if(moveDir.x == 1) {
    if(moveDir.y == 0) {
      //move right
      player.setFacing(Right);
      tempSpeed = levelSlot.validMove(player.getPos(), player.getSize(), player.getSpeed(), player.getFacing());
      player.setXPos(player.getXPos() + tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(moveDir.y == 1) {
      //down right
    }
    else if(moveDir.y == -1) {
      //up right
    }
  }
  else if(moveDir.x == -1) {
    if(moveDir.y == 0) {
      //move left
      player.setFacing(Left);
      tempSpeed = levelSlot.validMove(player.getPos(), player.getSize(), player.getSpeed(), player.getFacing());
      player.setXPos(player.getXPos() - tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(moveDir.y == 1) {
      //down left
    }
    else if(moveDir.y == -1) {
      //up left
    }
  }
  */

  sp = levelSlot.validMove(player.getPos(), player.getSize(), sp);
  player.setPos(player.getPos() + sp);
  //check for interaction between player and objects/entities

  //this might make more sense to be a separate function

  //for each element of the object/entity vectors, check if it intersects with the player
  //If so, do something...maybe run a callback from the object/entity
  for(int i=0;i<levelSlot.getObjNum();i++) {
    int j = i;
    //check for collision between player and levelSlot.getObject(i);
    Object ob{levelSlot.getObj(i)};
    sf::Vector2i pmin{player.getPos()};
    sf::Vector2i pmax{pmin+player.getSize()-sf::Vector2i(1,1)};

    sf::Vector2i omin{ob.getPos()};
    sf::Vector2i omax{omin+ob.getSize()-sf::Vector2i(1,1)};

    //I really think there is a better way to do this whole interaction thing

    if(pmin.x > omax.x || omin.x > pmax.x || pmin.y > omax.y || omin.y > pmax.y) {
      //no interaction
    }
    else {
      //interaction happens
      //what type of interaction is it?
      //use object id for this
      switch(ob.getId()) {
      case 0:
	//stone: push player out
	solidBehave(ob, this);
	break;
      case 1:

	pushBehave(ob, this);
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
      case 4:
	//cutscene player
	
	if(cutsceneManager.cutsceneExists(ob.getText())) {
	  cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(ob.getText()));
	  modeSwitcher.setMode(2);
	}
	break;
      default:
	//No interaction
	break;
      }
      if(i == j) {
	levelSlot.updateObj(i, ob);
      }
    //if(i >= levelSlot.getObjNum()) {
    //break;  // this is necessary until I find a better way, otherwise it attempts to read from removed nodes
    }
  }

  for(int i=0;i<levelSlot.getEntNum();i++) {
    //handle entities
    Entity en{levelSlot.getEnt(i)};
    sf::Vector2i pmin{player.getPos()};
    sf::Vector2i pmax{pmin+player.getSize()-sf::Vector2i(1,1)};

    sf::Vector2i emin{en.getPos()};
    sf::Vector2i emax{emin+en.getSize()-sf::Vector2i(1,1)};

    if(pmin.x > emax.x || emin.x > pmax.x || pmin.y > emax.y || emin.y > pmax.y) {
      //no interaction
    }
    else {
      //make player interact with entity
      switch(en.getType()) {
      default:
	//no interaction
	break;
      }
    }
  }

  sf::Vector2i oldps(player.getXScreen(), player.getYScreen());
  player.update(levelSlot.getTilesize());
  sf::Vector2i newps(player.getXScreen(), player.getYScreen());

  if(oldps != newps)
    levelSlot.displayUpdate = true;

  levelSlot.handleEntities();
  levelSlot.handleObjects(player.getPos(), player.getSize());

  std::string cn = levelSlot.getNode(player.getLevelXPos(levelSlot.getTilesizeX()), player.getLevelYPos(levelSlot.getTilesizeY())).getCutname();
  if(cn != "") {
    cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(cn));
    modeSwitcher.setMode(2);
  }
}


void MapData::event2Handle() {  //this is the cutscene mode
  if(cutscenePlayer.updateCutscene(player, message, levelSlot, modeSwitcher, musicPlayer, cutsceneManager)) {
    //I'm not sure if anything needs to go here
  }
  else {
    //cutscene is over; do things now... 
    modeSwitcher.setMode(1); //switch back to gameplay mode
  }
  sf::Vector2i oldps(player.getXScreen(), player.getYScreen());
  player.update(levelSlot.getTilesize());
  sf::Vector2i newps(player.getXScreen(), player.getYScreen());

  if(oldps != newps)
    levelSlot.displayUpdate = true;
}
