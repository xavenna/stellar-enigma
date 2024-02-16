#include "stellar-enigma.hpp"

// This file has functions for the overarching classes

MapData::MapData(unsigned pCool, unsigned mWid, unsigned mCool, unsigned mElem, unsigned mCharSize) : musicPlayer{"audiomap.txt"},  message{mWid, mCool, mElem, mCharSize}, cache{"assets/texturemap/default.tm", utility.save} {
  //initialize members here

  //load level
  if(!levelSlot.loadLevel("default")) {
    std::cerr << "Level load failed. Exiting\n";
    throw std::invalid_argument("MapData::MapData() : Level Load unsuccessful");
  }

  //set windowSize here
  int panOfX; //this is unused
  int msgOfY;
  windowSize = interface.initializeInterface(sf::Vector2i(WINDOW_WIDTH * levelSlot.getTilesize().x, WINDOW_HEIGHT * levelSlot.getTilesize().y), msgOfY, panOfX, &player, &cache);



  message.setPosition(4+levelSlot.getTilesize().x, msgOfY);
  

  cutscenePlayer.man.loadCutscenes("cutlist.txt");

  //load savedata.
  utility.save.setSlot("0");
  utility.save.readData();

  utility.save.writeData();
  //test line

  mainMenu.initialize();
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
  int code=0;
  unsigned keynum=0;
  while(modeSwitcher.getLastKey(lk)) {
    if(lk == sf::Keyboard::Enter) {
      code = mainMenu.getCode(0);
      keynum = 0;
      break;
    }
    else if(lk == sf::Keyboard::Num1) {
      code = mainMenu.getCode(1);
      keynum = 1;
      break;
    }
    else if(lk == sf::Keyboard::Num2) {
      code = mainMenu.getCode(2);
      keynum = 2;
      break;
    }
    else if(lk == sf::Keyboard::Num3) {
      code = mainMenu.getCode(3);
      keynum = 3;
      break;
    }
    else if(lk == sf::Keyboard::Num4) {
      code = mainMenu.getCode(4);
      keynum = 4;
      break;
    }
    else if(lk == sf::Keyboard::Escape) {
      code = mainMenu.getCode(5);
      keynum = 5;
      break;
    }
    else if(lk == sf::Keyboard::Space) {
      code = mainMenu.getCode(6);
      keynum = 6;
      break;
    }
  }
  switch(code) {
  case -1:
    //close program. Passed back to the main loop
    return -1;
  case 0:
    //do nothing
    break;
  case 1:
    //Enter mode 1. Leave map as-is
    modeSwitcher.setMode(1);
    modeSwitcher.cooldown(5);
    return 0; 
  case 2:
    //play cutscene, then switch to mode 1 as in code 1
    std::clog << "Playing cutscenes from menus is not yet supported\n";
    break;
  case 3:
    //load a different menu
    mainMenu.loadTemplate(mainMenu.getTextArg(keynum));

    //begin a cooldown
    modeSwitcher.cooldown(15);

    //as engine is already in mode 0, no switch needs to occur
    break;
  case 4:
    //enter mode 1, but load specified map
    loadLevel(mainMenu.getTextArg(keynum));
    modeSwitcher.setMode(1);
    modeSwitcher.cooldown(5);
    break;
  default:
    //if an unrecognized value is returned, print an error and exit
    std::clog << "Error: Invalid menu return code\n";
    return -1;
  }
  return 0;
}


void MapData::event1Handle() {
  //this is the basic gameplay mode
  sf::Keyboard::Key lk;
  sf::Vector2i moveDir(0,0);
  bool pause = false;
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
    if(lk == sf::Keyboard::Escape) {
      pause = true;
    }
  }
  if(pause) {
    //switch to mode 1
    modeSwitcher.cooldown(15);
    mainMenu.loadTemplate("pause");
    modeSwitcher.setMode(0);
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
      if(levelSlot.getObj(i).getStatus() == Object::Inactive || levelSlot.getObj(i).getStatus() == Object::Destroy) {
        continue;
      }
      for(unsigned j=i;j<levelSlot.getObjNum();j++) {
        if(i==j || levelSlot.getObj(j).getStatus() == Object::Inactive || levelSlot.getObj(j).getStatus() == Object::Destroy) {
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
        //obj-player interaction
        //call the obj's interact function on the player

        //check if the interaction was already resolved
        sf::Vector2i pmin{player.getPos()};
        sf::Vector2i pmax{pmin+player.getSize()-sf::Vector2i(1,1)};

        sf::Vector2i omin{x.o1->getPos()};
        sf::Vector2i omax{omin+x.o1->getSize()-sf::Vector2i(1,1)};

        if(pmin.x > omax.x || omin.x > pmax.x || pmin.y > omax.y || omin.y > pmax.y) {
          continue;
        }
        x.o1->savePos();
        player.savePos();

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
        x.o1->updateDelta();
        player.updateDelta();
      }
      else {

        //check if interaction was already resolved
        sf::Vector2i pmin{x.o1->getPos()};
        sf::Vector2i pmax{pmin+x.o1->getSize()-sf::Vector2i(1,1)};

        sf::Vector2i omin{x.o2->getPos()};
        sf::Vector2i omax{omin+x.o2->getSize()-sf::Vector2i(1,1)};

        if(pmin.x > omax.x || omin.x > pmax.x || pmin.y > omax.y || omin.y > pmax.y) {
          continue;
        }

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
        case Object::Entity:
          if(x.o2->Type() == Object::Static) {
            initiator = true;
          }
          else if(x.o2->Type() == Object::Entity || x.o2->Type() == Object::Sliding) {
            bool o1Moved = (delta1.x || delta1.y);
            bool o2Moved = (delta2.x || delta2.y);
            //std::cerr<<delta1.x<<','<<delta1.y<<';'<<delta2.x<<','<<delta2.y<<' ';
            //initiator = false; //this isn't always true. This should be lumped in with
                               //the sliding case and calculate based on deltas
            if(o1Moved && !o2Moved) {
              //o1 didn't move, o2 is initiator
              initiator = true;
            }
            else if(!o1Moved && o2Moved) {
              //o1 is initiator
              initiator = false;
            }
            else if(o1Moved && o2Moved) {
              //both objects moved. Obj with bigger delta wins
              continue;
            }
            else {
              //neither obj moved.
              //ignore interaction? i don't know what to do here ;-;
              continue; //this is a temporary fix...
            }
          }
          else {
            //...
          }
          break;
        }
        if(x.o2->Type() == Object::Intangible) {
          ignore = true;
        }

        if(!ignore) {
          x.o1->savePos();
          x.o2->savePos();
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
          x.o1->updateDelta();
          x.o2->updateDelta();
        }
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

  Interface res = levelSlot.handleObjects(player.getPos(), player.getSize(), &switchHandler, &utility);

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
  if(res.menu != "") {
    //open specified menu
    mainMenu.loadTemplate(res.menu);
    modeSwitcher.cooldown(15);
    modeSwitcher.setMode(0);
  }
  //objects and notifications are done in the Level, so they don't need to be done here


  // If player has died, display death screen and switch to mode 0 
  if(player.getHealth() == 0) {
    modeSwitcher.setMode(0);
    modeSwitcher.cooldown(15);
    mainMenu.loadTemplate("death");
  }
}


void MapData::event2Handle() {  //this is the cutscene mode
  if(cutscenePlayer.updateCutscene(player, message, levelSlot, modeSwitcher, musicPlayer, switchHandler)) {
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

void MapData::loadLevel(const std::string& name) {
  if(!levelSlot.loadLevel(name)) {
    //load was unsuccessful. Crash?
  }
  player.setHealth(5);
  player.setPos(32, 32);
  //reset all switches
  for(int i=0;i<255;i++) {
    switchHandler.write(i,false);
  }
  //set player position and data
}
