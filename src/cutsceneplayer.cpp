#include "cutsceneplayer.h"
#include <iostream>
#include <fstream>

// there should probably be a cutscene registry, similar to texturemap

void CutscenePlayer::playCutscene() {
  //this may be completely useless
  //actually, this could be used when switching to mode 2
}

bool CutscenePlayer::updateCutscene(Player& pl, ModeSwitcher& ms) {
  //make this start the next event, that would be pretty cool
  //okay, I think it does now. Yay
  Event e = cutscene.getEvent(pos);
  bool finalEvent = (cutscene.getListLen()-1 == (int)pos);
  //check if waiting for time or button press
  if(e.getType() == Event::GetInput) {
    //wait for player input
    sf::Keyboard::Key k;
    while(ms.getLastKey(k)) {
      //check if key is valid
      if(e.getArg(1) == 1 || (e.getArg(1) == 0 && e.getArg(0) == k)) {
	//event is over
	if(finalEvent) {
	  //cutscene is over, return or whatever
	  return false;
	}
	else {
	  pos++;
	  //start next event?
	  if(!playEvent(pl, ms)) {
	    //error: invalid event
	    std::cout << "Error: invalid event in cutscene. Event skipped.\n";
	  }
	}
      }
    }
  }
  else {
    //waiting for time
    if(timer == 0) {
      //event is over
      if(finalEvent) {
	//cutscene is over, return or whatever
	return false;
      }
      else {
	pos++;
	//start next event?
	if(!playEvent(pl, ms)) {
	  //error: invalid event
	  std::cout << "Error: invalid event in cutscene. Event skipped.\n";
	  //set timer to 0?
	}
      }
    }
    else {
      timer--;
    }
    ms.ignoreKeypresses();
  }
  return true;
}
bool CutscenePlayer::playEvent(Player& pl, ModeSwitcher& ms) {  //add bounds checking to this
  Event e = cutscene.getEvent(pos);
  switch(e.getType()) {
    //currently, only plyerMove does anything
  case Event::PlayerMove:
    //move player
    pl.setPos(e.getArg(0), e.getArg(1));
    timer = e.getDuration();
    break;
  case Event::EntityMove:
    //move entity
    timer = e.getDuration();
    break;
  case Event::ObjectRemove:
    //remove object
    timer = e.getDuration();
    break;
  case Event::ObjectPlace:
    //place object
    timer = e.getDuration();
    break;
  case Event::MessageDisplay:
    //display message
    timer = e.getDuration();
    break;
  case Event::ImageDisplay:
    //display image
    //this is currently unimplemented
    return false;
  case Event::GetInput:
    //wait for input
    //nothing needs to happen here
    break;
  }
  return true;
}
void CutscenePlayer::loadCutscene(const Cutscene& cut) {
  cutscene = cut;
  pos = 0;
  
}
CutscenePlayer::CutscenePlayer() {
  timer = 0;
  pos = 0;
}

