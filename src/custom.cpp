#include "stellar-enigma.hpp"
#include "mapdata.h"

void MapData::customInit() {
  //this function is where any custom code to be run before the main loop should go
  modeSwitcher.setMode(0);
  player.setXPos(36);
  player.setYPos(36);
  player.setSpeed(18);
  sf::SoundBuffer step;
  step.loadFromFile("assets/audio/thud.wav");
  musicPlayer.registerSound("step", step);
  //cutscene
  Cutscene cut;
  cut.loadFromFile("assets/cutscene/text.sec");
  cutscenePlayer.loadCutscene(cut);
}

void initialSetup(std::string& name, int& width, int& height, int& framerate) {
  name = "Stellar Enigma Engine Test";  //this is the name of the window
  width = 416;  //this is the width of the window, in pixels
  height = 416;  //this is the height of the window
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
void MapData::event1Handle() {  //this is the primary mode
  int oldXScr = player.getXScreen();
  int oldYScr = player.getYScreen();
  sf::Keyboard::Key lk;
  while(modeSwitcher.getLastKey(lk)) {
    int tempSpeed;
    if(lk == sf::Keyboard::Quote) {
      //query message
      std::cout << message.getMessage() << '\n';
    }
    if(lk == sf::Keyboard::W) {
      player.setFacing(Up);
      tempSpeed = validMove(levelSlot, player);
      player.setYPos(player.getYPos() - tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::A) {
      player.setFacing(Left);
      tempSpeed = validMove(levelSlot, player);
      player.setXPos(player.getXPos() - tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::S) {
      player.setFacing(Down);
      tempSpeed = validMove(levelSlot, player);
      player.setYPos(player.getYPos() + tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::D) {
      player.setFacing(Right);
      tempSpeed = validMove(levelSlot, player);
      player.setXPos(player.getXPos() + tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	musicPlayer.queueSound("step");
      }
    }
  }
  //post-handling thingies
  player.update(levelSlot.getTilesizeX(), levelSlot.getTilesizeY());
  if(oldXScr != player.getXScreen() || oldYScr != player.getYScreen()) {
    levelSlot.displayUpdate = true;
  }
  if(levelSlot.getNode(player.getLevelXPos(levelSlot.getTilesizeX()), player.getLevelYPos(levelSlot.getTilesizeY())).getId() == 7) {
    message.addMessage("I see you found a key... Cool. It does nothing");
    //so, currently this will add a message every frame..., which is less than ideal. Fix this!
  }
  if(levelSlot.getNode(player.getLevelXPos(levelSlot.getTilesizeX()), player.getLevelYPos(levelSlot.getTilesizeY())).getId() == 9) {
    //messageboard
    modeSwitcher.setMode(2);
  }
}


void MapData::event2Handle() {  //this is the cutscene mode
  if(cutscenePlayer.updateCutscene(player, modeSwitcher)) {
    //I'm not sure if anything needs to go here
  }
  else {
    //cutscene is over; do things now... 
    modeSwitcher.setMode(1); //switch back to gameplay mode
  }

}
