#include "stellar-enigma.hpp"

void customInit(MapData& md) {
  md.modeSwitcher.setMode(0);
  md.player.setXPos(36);
  md.player.setYPos(36);
  md.player.setSpeed(18);
  //register audio?
  sf::SoundBuffer step;
  step.loadFromFile("assets/audio/thud.wav");
  md.musicPlayer.registerSound("step", step);
}

void initialSetup(std::string& name, int& width, int& height, int& framerate) {
  name = "Stellar Enigma Engine Test";  //this is the name of the window
  width = 416;  //this is the width of the window, in pixels
  height = 416;  //this is the height of the window
  framerate = 30;   //the framerate
}

void event0Handle(MapData& md) {  //this mode is used for the main menu
  sf::Keyboard::Key lk;
  while(md.modeSwitcher.getLastKey(lk)) {
    if(lk == sf::Keyboard::Enter) {
      md.modeSwitcher.setMode(1);
    }
  }
  
}
void event1Handle(MapData& md) {  //this is the primary mode
  int oldXScr = md.player.getXScreen();
  int oldYScr = md.player.getYScreen();
  sf::Keyboard::Key lk;
  while(md.modeSwitcher.getLastKey(lk)) {
    int tempSpeed;
    if(lk == sf::Keyboard::Quote) {
      //query message
      std::cout << md.message.getMessage() << '\n';
    }
    if(lk == sf::Keyboard::W) {
      md.player.setFacing(Up);
      tempSpeed = validMove(md.levelSlot, md.player);
      md.player.setYPos(md.player.getYPos() - tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	md.musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::A) {
      md.player.setFacing(Left);
      tempSpeed = validMove(md.levelSlot, md.player);
      md.player.setXPos(md.player.getXPos() - tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	md.musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::S) {
      md.player.setFacing(Down);
      tempSpeed = validMove(md.levelSlot, md.player);
      md.player.setYPos(md.player.getYPos() + tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	md.musicPlayer.queueSound("step");
      }
    }
    else if(lk == sf::Keyboard::D) {
      md.player.setFacing(Right);
      tempSpeed = validMove(md.levelSlot, md.player);
      md.player.setXPos(md.player.getXPos() + tempSpeed);
      if(tempSpeed > 0) {
	//move succeeded
	md.musicPlayer.queueSound("step");
      }
    }
  }
  //post-handling thingies
  md.player.update(md.levelSlot.getTilesizeX(), md.levelSlot.getTilesizeY());
  if(oldXScr != md.player.getXScreen() || oldYScr != md.player.getYScreen()) {
    md.levelSlot.displayUpdate = true;
  }
  if(md.levelSlot.getNode(md.player.getLevelXPos(md.levelSlot.getTilesizeX()), md.player.getLevelYPos(md.levelSlot.getTilesizeY())).getId() == 7) {
    //figure out how to convert player position(fine) to map position
    md.message.addMessage("I see you found a key... Cool. It does nothing");
    //so, currently this will add a message every frame..., which is less than ideal. Fix this!
  }

}
