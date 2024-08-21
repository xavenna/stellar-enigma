#include "cutscene/cutsceneplayer.h"

void CutscenePlayer::playCutscene() {//this may be completely useless
  //actually, this could be used when switching to mode 2
}

bool CutscenePlayer::updateCutscene() {
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
          if(!playEvent()) {
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
        if(!playEvent()) {
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

bool CutscenePlayer::playEvent() {
  //add bounds checking whenever an arg is used as an index, or similar use.
  Event e = cutscene.getEvent(pos);
  switch(e.getType()) {
  case Event::UpdateNode:
    //update a node on the map
    //e[0] - xpos
    //e[1] - ypos
    //e[2] - id
    //e[3] - solidity: represented as a directional Bool
    
    //somehow check for invalid args
    std::clog << "Note: UpdateNode event has no bounds checking yet. Use at your own risk\n";

    le.updateNode(e[0], e[1], MapNode(static_cast<unsigned>(e[2]), DirectionalBool(e[3])));
    timer = e.getDuration();
    le.displayUpdate = true;
    break;
  case Event::WriteSwitch:
    //write value of e[1] to switch e[0]
    //validate args
    if(e[0] > 255 || e[0] < 0) {
      //invalid index
      std::cerr << "Error: invalid argument in WriteSwitch event in cutscene. Skipped\n";
    }
    else {
      //e[0] is guaranteed to be between 0 and 255
      sh.write(e[0], static_cast<bool>(e[1])); 
    }
    timer = e.getDuration();
    break;
  case Event::DisplayMessage:
    //display message
    me.addMessage(e.getText());
    timer = e.getDuration();
    break;
  case Event::GetInput:
    //wait for input
    //nothing needs to happen here
    break;
  case Event::PlaySound:
    //plays a sound
    mp.queueSound(e.getText());
    timer = e.getDuration();
    break;
  case Event::SwitchMusic:
    //plays music
    mp.playMusic(e.getText());
    timer = e.getDuration();
    break;
  case Event::MovePlayer:
    //e[0] - is motion relative?
    //e[1] - should collision and interaction be ignored?
    //e[2] - x distance/position
    //e[3] - y distance/position

    if(e[0]) {
      //relative coords for pos
      if(e[1]) {
        //ignore collision
        pl.setPos((pl.getPos().x + e[2]), (pl.getPos().y + e[3]));
      }
      else {
        //don't ignore collision
        //use e[2], e[3] as vector, call le.validMove()
        //do later
        std::cerr << "Not implemented\n";
      }
    }
    else {
      //absolute coords for pos
      //collision is always ignored for absolute motion.
      pl.setPos(e[2], e[3]);
    }

    timer = e.getDuration();
    break;
  case Event::MoveObject:
    //e[0] - is motion relative?
    //e[1] - should collision and interaction be ignored?
    //e[2] - x distance/position
    //e[3] - y distance/position
    //e[4] - link id of object to move
    if(!le.hasObj(e[4])) {
      //object doesn't exist, fail
      std::cerr << "Specified Object wasn't found in cutscene, Skipping event.\n";
    }
    else {
      Object* obj = le.getObjLinkPtr(e[4]);
      if(e[0]) {
        //relative coords for pos
        if(e[1]) {
          //ignore collision
          obj->setPos((pl.getPos().x + e[2]), (pl.getPos().y + e[3]));
        }
        else {
          //don't ignore collision
          //use e[2], e[3] as vector, call le.validMove()
          //do later
          std::cerr << "Not implemented\n";
        }
      }
      else {
        //absolute coords for pos
        //collision is always ignored for absolute motion.
        obj->setPos(e[2], e[3]);
      }
    }

    timer = e.getDuration();
    break;
  case Event::NotifyObject:
    //send message: e[0] is linkid, e[1] is type, e[2]-e[7] are data.
    //create msg
    if(!le.hasObj(e[0])) {
      std::cerr << "Error: Specified object not found in cutscene event, skipped.\n"; 
    }
    else {
      msg m;
      m.link = static_cast<unsigned>(e[0]);
      m.type = static_cast<char>(e[1]);
      for(unsigned i=0;i<6;i++) {
        //copy data to msg
        m.data[i] = e[i+2];
      }
      le.notifyObj(m);
    }
    break;
  /*
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
    le.updateNode(e[0], e[1], MapNode(static_cast<unsigned>(e[2]), DirectionalBool(e[3])));
    le.displayUpdate = true;
    break;
  case Event::MapLoad:
    //load a new map
    le.loadLevel(e.getText());
    man.loadCutscenes(e.getText());
    pl.setPos(e[0],e[1]);
    break;
  */
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
CutscenePlayer::CutscenePlayer(Player& p, Message& m, Level& l, ModeSwitcher& mo, MusicPlayer& mu, SwitchHandler& s, Camera& c) : pl{p}, me{m}, le{l}, ms{mo}, mp{mu}, sh{s}, cam{c} {
  timer = 0;
  pos = 0;
}

