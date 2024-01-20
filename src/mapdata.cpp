#include "stellar-enigma.hpp"

// This file has functions for the overarching classes

MapData::MapData(unsigned pCool, unsigned mWid, unsigned mCool, unsigned mElem, unsigned mCharSize) :  message{mWid, mCool, mElem, mCharSize}, cache{"assets/texturemap/default.tm", save} {
  //initialize members here

  //load level
  try {
    levelSlot.loadLevel("default");
  }
  catch (int e) {  //make these error codes better
    if(e == 0) {
      std::cout << "Error 0: Level not found\n";
    }
    else if(e == -1) {
      std::cout << "Error -1: Non-integer found in integer parameter\n";
    }
    else if(e == -2) {
      std::cout << "Error: -2\n";
    }
    else if(e == -3) {
      std::cout << "Error: -3\n";
    }
    throw e;
  }

  //set windowSize here
  int panOfX; //this is unused
  int msgOfY;
  windowSize = interface.initializeInterface(sf::Vector2i(WINDOW_WIDTH * levelSlot.getTilesize().x, WINDOW_HEIGHT * levelSlot.getTilesize().y), msgOfY, panOfX, &player, &cache);



  message.setPosition(4+levelSlot.getTilesize().x, msgOfY);
  

  cutscenePlayer.man.loadCutscenes("default");

  //load savedata.
  save.setSlot("0");
  save.readData();

  save.writeData();
  //test line
}

unsigned long MapData::getFrameCount() const {
  return frameCount;
}

void MapData::pollEvents(sf::RenderWindow& window) {
    sf::Event event;
    while(window.pollEvent(event)) {
      switch(event.type) {
      case sf::Event::Closed:
	      window.close();
	      break;
      default:
	      break;
      }
    }

    modeSwitcher.updateKeysPressed();
}
void MapData::finishFrame(sf::RenderWindow& window) {
  //tasks that are common to all modes
  musicPlayer.assignSounds();
  musicPlayer.manageSounds();
  interface.updateInterface(&player, &cache);

  window.clear();
  //draw based on which mode the engine is currently in
  switch(modeSwitcher.getMode()) {
  case 0:
    //menu mode
    //assign texture to mainMenu

    mainMenu.assignTexture(cache);
    window.draw(mainMenu);
    break;
  case 1:
  case 2:  //cutscene mode works the same as gameplay mode
  case 3:  //debug mode can use the same drawing logic as normal mode
    //assign sprites
    if(levelSlot.displayUpdate) {
      levelSlot.readyWindow(player.getScreen().x, player.getScreen().y);
    }
    for(unsigned i=0;i<WINDOW_WIDTH;i++) {
      for(unsigned j=0;j<WINDOW_HEIGHT;j++) {
        levelSlot.assignTextureToWinNode(sf::Vector2i(static_cast<int>(i),static_cast<int>(j)), cache);
        window.draw(levelSlot.window[i][j]);
      }
    }

    for(unsigned i=0;i<levelSlot.getObjNum();i++) {
      levelSlot.assignTextureToObject(i, cache);
      if(levelSlot.displayObject(i, player.getPos(), player.getSize())) {
        window.draw(levelSlot.getObjRef(i));
      }
    }

    //update things
    player.update(levelSlot.getTilesize());
    player.assignTexture(cache);
    window.draw(player);

    //draw icons
    interface.drawIcons(window);

    //draw message
    message.handleMessages();
    message.wrapMessage();
    window.draw(message);

    //draw border
    for(unsigned i=0;i<interface.getBorderLen();i++) {
      window.draw(interface.getBorderElem(i));
    }
    break;
  default:
    //any other modes would go here
    break;
  }
  window.display();
  levelSlot.advanceFrameCount();
  frameCount++;
}

int MapData::handleEvents() {
  switch(modeSwitcher.getMode()) {
  case 0:
    return event0Handle();
    break;
  case 1:
    event1Handle();
    break;
  case 2:
    event2Handle();
    break;
  case 3:
    event3Handle();
    break;
  case 4:
    //event4Handle();
    break;
  default:
    //don't handle any events... in an unrecognised mode
    break;

  }
  return 0;
}


// Handles events in mode 0 (menus and splash screens)
int MapData::event0Handle() {
  sf::Keyboard::Key lk;
  while(modeSwitcher.getLastKey(lk)) {
    if(lk == sf::Keyboard::Enter) {
      switch(mainMenu.onPress) {
      case 1:
        modeSwitcher.setMode(1);
        return 0; 
      case -1:
        return -1;
      default:
        //if an unrecognized value is returned, print an error and exit
        std::cout << "Error: Invalid menu return code\n";
        return -1;
      }
    }
  }
  return 0;
}


void MapData::event1Handle() {
  //this is the basic gameplay mode
  sf::Keyboard::Key lk;
  sf::Vector2i moveDir(0,0);
  while(modeSwitcher.getLastKey(lk)) {
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
  //now that all input has been gathered, handle movement
  sf::Vector2i sp(moveDir.x * player.getSpeed(), moveDir.y * player.getSpeed());


  sp = levelSlot.validMove(player.getPos(), player.getSize(), sp);
  player.setPos(player.getPos() + sp);
  //check for interaction between player and objects/entities


  //set up interaction handler

  std::deque<Inter> interactions;


  //iterator
  for(unsigned n=0;;n++) {
    interactions.clear();
    //create a list of interactions
    //check each pair of objects (plus player)
    for(unsigned i=0;i<levelSlot.getObjNum();i++) {
      if(!levelSlot.getObj(i).getActive()) {
        continue;
      }
      for(unsigned j=i;j<levelSlot.getObjNum();j++) {
        if(i==j || !levelSlot.getObj(j).getActive()) {
          continue;
        }
        //check interaction between objects
        Object* o1 = (levelSlot.getObjPtr(i));
        Object* o2 = (levelSlot.getObjPtr(j));

        sf::Vector2i omin{o1->getPos()};
        sf::Vector2i omax{omin+o1->getSize()-sf::Vector2i(1,1)};

        sf::Vector2i bmin{o2->getPos()};
        sf::Vector2i bmax{bmin+o2->getSize()-sf::Vector2i(1,1)};


        if(bmin.x > omax.x || omin.x > bmax.x || bmin.y > omax.y || omin.y > bmax.y) {
          //no interaction
        }
        else {
          //interaction;
          //create and populate an Inter object
          interactions.push_back(Inter(o1, o2));
        }
      }
      //check interaction with player and object[i];
      Object* ob = (levelSlot.getObjPtr(i));
      sf::Vector2i pmin{player.getPos()};
      sf::Vector2i pmax{pmin+player.getSize()-sf::Vector2i(1,1)};

      sf::Vector2i omin{ob->getPos()};
      sf::Vector2i omax{omin+ob->getSize()-sf::Vector2i(1,1)};

      if(pmin.x > omax.x || omin.x > pmax.x || pmin.y > omax.y || omin.y > pmax.y) {
        //no interaction
      }
      else {
        //interaction
        interactions.push_back(Inter(ob, player));
      }
    }
    //calculate interaction ranking
    for(auto& x : interactions) {
      x.calculatePriority();
    }

    //reorder interaction vector

    //sorting by subpriority then priority with a stable sort ensures that the vector
    //is lexicographically sorted

    std::stable_sort(interactions.begin(), interactions.end(), [](Inter o, Inter p) {
        return o.subpriority < p.subpriority;
    });

    std::stable_sort(interactions.begin(), interactions.end(), [](Inter o, Inter p) {
        return o.priority < p.priority;
    });

    //handle in priority rank
    for(size_t i=0;i<interactions.size();i++) {
      bool remove = false;
      auto& x = interactions[i];
      if(x.player1 || x.player2) {
        x.o1->savePos();
        player.savePos();
        //obj-player interaction
        //call the obj's interact function on the player

        Interface res = x.o1->interact(&player, &levelSlot.field, &switchHandler);
        for(auto y : res.message) {
          if(y != "") {
            message.addMessage(y);
          }
        }
        for(auto y : res.cutscene) {
          if(y != "") {
            if(cutscenePlayer.man.cutsceneExists(y)) {
              cutscenePlayer.loadCutscene(cutscenePlayer.man.getCutscene(y));
            }
            modeSwitcher.setMode(2);
          }
        }
        for(auto y : res.sounds) {
          musicPlayer.queueSound(y);
        }

        for(auto y : res.objs) {
          levelSlot.addObject(y.first, y.second);
        }

        if(x.o1->getStatus() == Object::Destroy) {
          //don't destroy the object yet, wait until the end of the process
          //levelSlot.removeObject(x.o1);
          //remove = true;
        }
        x.o1->updateDelta();
        player.updateDelta();
      }
      else {
        x.o1->savePos();
        x.o2->savePos();
        bool initiator = true; //if true, o1 initiates
        bool ignore = false; //if true, ignore interaction
        //obj-obj interaction
        sf::Vector2i delta1 = x.o1->getPos() - x.o1->getLastPos();
        sf::Vector2i delta2 = x.o2->getPos() - x.o2->getLastPos();

        //call target's interact function on initiator object
        //if one is a static, it is the target
        //if both are static, ignore interaction
        //if one is an entity, it is the initiator
        //if one is a sliding, it is the initiator
        //if one is entity and one is sliding, entity initiates
        //if both are entity or sliding, figure something out;
        //check deltas. If one has a delta but the other doesn't, one with delta is
          //initiator
        //if both have deltas... i have no ideas
        switch(x.o1->Type()) {
        default:
          //treat as intangible
        case Object::Intangible:
          ignore = true;
          break;
        case Object::Static:
          if(x.o2->Type() == Object::Static) {
            //ignore interaction
            ignore = true;
          }
          else if(x.o2->Type() == Object::Sliding) {
            initiator = false;
          }
          else {
            initiator = false;
          }
          break;
        case Object::Sliding:
          if(x.o2->Type() == Object::Static) {
            initiator = true;
          }
          else if(x.o2->Type() == Object::Entity) {
            initiator = false;
          }
          else {
            //complicated things
          }
          break;
        case Object::Entity:
          if(x.o2->Type() == Object::Static) {
            initiator = true;
          }
          else if(x.o2->Type() == Object::Sliding) {
            initiator = true;
          }
          else {
            //complicated things
          }
          break;
        }
        if(x.o2->Type() == Object::Intangible) {
          ignore = true;
        }

        if(!ignore) {
          Interface res = (initiator?x.o2:x.o1)->interact((initiator?x.o1:x.o2), &levelSlot.field, &switchHandler);
          for(auto y : res.message) {
            if(y != "") {
              message.addMessage(y);
            }
          }
          for(auto y : res.cutscene) {
            if(y != "") {
              if(cutscenePlayer.man.cutsceneExists(y)) {
                cutscenePlayer.loadCutscene(cutscenePlayer.man.getCutscene(y));
              }
              modeSwitcher.setMode(2);
            }
          }
          for(auto y : res.sounds) {
            musicPlayer.queueSound(y);
          }

          for(auto y : res.objs) {
            //create any requested objects
            levelSlot.addObject(y.first, y.second);
          }

          if(x.o1->getStatus() == Object::Destroy) {
            levelSlot.removeObject(x.o1);
          }
          if(x.o2->getStatus() == Object::Destroy) {
            levelSlot.removeObject(x.o2);
          }
          x.o1->updateDelta();
          x.o2->updateDelta();
        }
      }
    }

    for(unsigned i=0;i<levelSlot.getObjNum();i++) {
      auto p = levelSlot.getObj(i);
      if(p.getStatus() == Object::Destroy) {
        levelSlot.removeObject(i);
        i--;
      }
    }

    if(interactions.empty() || n >= 6) {
      // the number is a hardcap for iterations, it may change
      // also break if the list remains identical for several iterations in a row
      // i'm not sure what the best way to do this is...
      break;
    }

  }


  //various player updates
  if(player.damaged) {
    player.damaged = false;
    player.resetCooldown();
  }
  else if(player.getCooldown() != 0) {
    player.decrementCooldown();
  }
  sf::Vector2i oldps(player.getScreen().x, player.getScreen().y);
  player.update(levelSlot.getTilesize());
  sf::Vector2i newps(player.getScreen().x, player.getScreen().y);

  //various updates
  if(oldps != newps)
    levelSlot.displayUpdate = true;

  Interface res = levelSlot.handleObjects(player.getPos(), player.getSize(), &switchHandler);

  //handle the interfaces
  for(auto x : res.message) {
    if(x != "") {
      message.addMessage(x);
    }
  }
  for(auto x : res.cutscene) {
    if(x != "") {
      if(cutscenePlayer.man.cutsceneExists(x)) {
        cutscenePlayer.loadCutscene(cutscenePlayer.man.getCutscene(x));
      }
      modeSwitcher.setMode(2);
    }
  }
  for(auto x : res.sounds) {
    musicPlayer.queueSound(x);
  }
  //objects and notifications are done in the Level, so they don't need to be done here


  // If player has died, display death screen and switch to mode 0 
  if(player.getHealth() == 0) {
    modeSwitcher.setMode(0);
    mainMenu.baseImage = "blanksplash";
    mainMenu.tList.clear();
    Transform t;
    t.type = Transform::Add_Text;
    t.text = "You Died :(\nPress Enter to Quit";
    t.args[0] = 8; //change this once the texture base is made
    t.args[1] = 0; //change this once the texture base is made
    t.args[2] = 20; //size, tweak until it looks good
    mainMenu.tList.push_back(t);
    mainMenu.onPress = -1;
  }
}


void MapData::event2Handle() {  //this is the cutscene mode
  if(cutscenePlayer.updateCutscene(player, message, levelSlot, modeSwitcher, musicPlayer)) {
    //I'm not sure if anything needs to go here
  }
  else {
    //cutscene is over; do things now... 
    modeSwitcher.setMode(1); //switch back to gameplay mode
  }
  sf::Vector2i oldps(player.getScreen().x, player.getScreen().y);
  player.update(levelSlot.getTilesize());
  sf::Vector2i newps(player.getScreen().x, player.getScreen().y);

  if(oldps != newps)
    levelSlot.displayUpdate = true;
}


void MapData::event3Handle() {  //debug mode logic
  return; //no logic occurs
}
