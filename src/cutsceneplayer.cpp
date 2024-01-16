#include "cutsceneplayer.h"
#include <iostream>
#include <fstream>

// there should probably be a cutscene registry, similar to texturemap

void CutscenePlayer::playCutscene() {//this may be completely useless
  //actually, this could be used when switching to mode 2
}

bool CutscenePlayer::updateCutscene(Player& pl, Message& me, Level& le, ModeSwitcher& ms, MusicPlayer& mp, CutsceneManager& cm) {
  //make this start the next event, that would be pretty cool
  //okay, I think it does now. Yay
  Event e = cutscene.getEvent(pos);
  bool finalEvent = (cutscene.getListLen()-1 == static_cast<int>(pos));
  //check if waiting for time or button press
  if(e.getType() == Event::GetInput) {
    //wait for player input
    sf::Keyboard::Key k;
    while(ms.getLastKey(k)) {
      //check if key is valid
      if(e[1] == 1 || (e[1] == 0 && e[0] == k)) {
        //event is over
        if(finalEvent) {
          //cutscene is over, return or whatever
          return false;
        }
        else {
          pos++;
          //start next event?
          if(cutscene.getListLen()-1 == static_cast<int>(pos)) {
            return false;
          }
          if(!playEvent(pl, me, le, mp, cm)) {
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
        pos = 0;
        return false;
      }
      else {
        pos++;
        //start next event?
        if(!playEvent(pl, me, le, mp, cm)) {
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

bool CutscenePlayer::playEvent(Player& pl, Message& me, Level& le, MusicPlayer& mp, CutsceneManager& cm) {
  //add bounds checking to this
  Event e = cutscene.getEvent(pos);
  switch(e.getType()) {
  case Event::PlayerUpdate:
    //move player
    pl.setPos(e[0], e[1]);
    pl.setSpeed(e[2]);
    pl.setFacing(static_cast<Direction>(e[3]));
    //if text isn't null, then set texture to
    //"assets/texture/"+e.getText()+".png"
    //set width and height to texture.getSize().x & texture.getSize().y
    timer = e.getDuration();
    break;
  case Event::EntityMove:
    //move entity
    return false;
    timer = e.getDuration();
    break;
  case Event::ObjectRemove:
    //remove object
    return false;
    timer = e.getDuration();
    break;
  case Event::ObjectPlace:
    //place object
    std::clog << "The Event::ObjectPlace event type is deprecated\n";

    //create object
    //add it to list
    //DEPRECATED
    //le.addObject(Object(e[0], e[1], e[2], e[3], e[4], e[5], e[6], e.getText()));
    //timer = e.getDuration();
    break;
  case Event::MessageDisplay:
    //display message
    me.addMessage(e.getText());
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
  case Event::NodeUpdate:
    //update a node on the map
    //std::cout << "updating node\n";
    le.updateNode(e[0], e[1], MapNode(static_cast<unsigned>(e[2]), DirectionalBool(e[3]), e.getText()));
    le.displayUpdate = true;
    break;
  case Event::SoundPlay:
    //plays a sound
    mp.queueSound(e.getText());
    break;
  case Event::MusicPlay:
    //plays music
    mp.playMusic(e.getText());
    break;
  case Event::MapLoad:
    //load a new map
    le.loadLevel(e.getText());
    cm.loadCutscenes(e.getText());
    pl.setPos(e[0],e[1]);
    break;
  case Event::Invalid:
  default:
    //error: invalid event
    std::cout << "Error: Invalid Event\n";
    return false;
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

