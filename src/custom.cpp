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
  cutsceneManager.loadCutscenes("assets/cutscene/cutlist.txt");

  mainMenu.spT.loadFromFile("assets/splash/mainmenu.png");
  mainMenu.splash.setTexture(mainMenu.spT);  //this should likely go somewhere
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
void MapData::event1Handle() {
  //this is the primary mode
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
  //fix the algorithm for determining when this screen rolls over
  player.update(levelSlot.getTilesizeX(), levelSlot.getTilesizeY());
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
  if(cutscenePlayer.updateCutscene(player, message, levelSlot, modeSwitcher, musicPlayer)) {
    //I'm not sure if anything needs to go here
  }
  else {
    //cutscene is over; do things now... 
    modeSwitcher.setMode(1); //switch back to gameplay mode
  }
  player.update(levelSlot.getTilesizeX(), levelSlot.getTilesizeY());
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
