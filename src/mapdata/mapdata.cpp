#include "mapdata/mapdata.h"

// This file has functions for the overarching classes

MapData::MapData(unsigned mWid, unsigned mCool, unsigned mElem, unsigned mCharSize) : musicPlayer{"audiomap.txt"},  message{mWid, mCool, mElem, mCharSize}, camera{player, levelSlot, "assets/camera/config.json"}, cutscenePlayer{player, message, levelSlot, modeSwitcher, musicPlayer, switchHandler, camera}, cache{"assets/texturemap/default.tm", utility.save} {
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
  

  cutscenePlayer.man.loadCutscenes("assets/cutscene/cutscenes.json");

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
#ifndef SE_RAW_IN
      case sf::Event::KeyPressed:
        modeSwitcher.addKey(event.key.code);
        break;
#endif
      default:
	      break;
      }
    }

#ifdef SE_RAW_IN
    modeSwitcher.updateKeysPressed();
#endif
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

    for(unsigned i=0;i<levelSlot.getObjNum();i++) {
      levelSlot.assignTextureToObject(i, cache);
    }
    camera.drawFrame(window, modeSwitcher.getMode(), cache);

    //update things
    player.update();
    player.assignTexture(cache);
    //window.draw(player);

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
    event4Handle();
    break;
  case 5:
    event5Handle();
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
  sf::Vector2f moveDir(0,0);
  bool pause = false;
  bool interact = false;
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
    else if(lk == sf::Keyboard::Escape) {
      pause = true;
    }
    else if(lk == sf::Keyboard::K) {
      interact = true;
    }

  }
  if(pause) {
    //switch to mode 1
    modeSwitcher.cooldown(15);
    mainMenu.loadTemplate("pause");
    modeSwitcher.setMode(0);
  }


  //now that all input has been gathered, handle movement
  if(std::fabs(moveDir.x) == 1 && std::fabs(moveDir.y) == 1) {
    //scale diagonal movement by sqrt(2)/2 so that diagonal moves aren't longer
    moveDir *= static_cast<float>(sqrt(2)/2);
  }


  sf::Vector2f sp(moveDir.x * player.getSpeed(), moveDir.y * player.getSpeed());
  player.setSelfPush(sp);

  //move held object
  if(!levelSlot.hasObjPtr(player.getHeldObj())) {
    //if object no longer exists, don't do this
    player.grabbing = false;
  }
  else if(player.grabbing) {
    Object* o = player.getHeldObj();
    o->setSelfPush(sp);
  }


  //set player facing direction now, to avoid breaking pushing
  //by ordering it like this, diagonal movement faces sideways.

  if(sp.x < 0) {
    player.setFacing(Left);
  }
  else if(sp.x > 0) {
    player.setFacing(Right);
  }
  else if(sp.y < 0) {
    player.setFacing(Up);
  }
  else if(sp.y > 0) {
    player.setFacing(Down);
  }
  //if no movement, don't adjust dir


  //set up interaction handler

  handleInteractions();
  if(player.getActCooldown() != 0) {
    player.decrementActCooldown();
  }
  if(interact && player.getActCooldown() == 0) {
    player.resetActCooldown();
    //set player to interacting mode. Determine the interact target, and set its
    //status to interacted

    //search for an object to interact with
    //using player's facing direction, search a rectangle size 8x8, centered 6 units
    //in front of player's facing side.
    //get a list of every obj that intersects with this range
    sf::FloatRect searchBox;

    searchBox.left = player.getCenter().x;
    searchBox.top = player.getCenter().y;
    if(player.getFacing() == Up) {
      searchBox.top -= (6 + player.getSize().y / 2.f);
    }
    else if(player.getFacing() == Down) {
      searchBox.top += (6 + player.getSize().y / 2.f);
    }
    else if(player.getFacing() == Left) {
      searchBox.left -= (6 + player.getSize().y / 2.f);
    }
    else if(player.getFacing() == Right) {
      searchBox.left += (6 + player.getSize().y / 2.f);
    }

    searchBox.width = 6.f;
    searchBox.height = 6.f;
    std::vector<Object*> inter_list;

    //find all objects that can be grabbed
    for(unsigned i=0;i<levelSlot.getObjNum();i++) {
      if(levelSlot.getObjPtr(i)->getBounds().intersects(searchBox)) {
        inter_list.push_back(levelSlot.getObjPtr(i));
      }
    }


    if(inter_list.size() != 0) {

      std::stable_sort(inter_list.begin(), inter_list.end(), [](Object* o, Object* p) {
          return o->priority() < p->priority();
      });

      Object* target = inter_list[0];

      //what do we do now?
      if(target->use(&player)) {
        if(player.grabbing == false) {
          //player grabbed object...?
          target->held = true;
          player.grabbing = true;
          player.setHeldObj(target);
          std::cerr << "Grabbing obj\n";
        }
        else {
          target->held = false;
          player.grabbing = false;
          std::cerr << "Releasing obj\n";
        }
      }
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
  //this is no longer needed?
  player.update();

  //various updates

  Interface res = levelSlot.handleObjects(&switchHandler, &utility);

  //handle the interfaces
  for(auto x : res.message) {
    if(x != "") {
      message.addMessage(x);
    }
  }
  for(auto x : res.cutscene) {
    if(x != "") {
      cutscenePlayer.playCutscene(x);
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


void MapData::handleInteractions() {
  //first, update each object's position using the internal selfPush vector;
  for(unsigned i=0;i<levelSlot.getObjNum();i++) {
    auto* x = levelSlot.getObjPtr(i);
    sf::Vector2f fspeed = levelSlot.validMove(x->getPos(), x->getSize(), x->getSelfPush());
    x->setPos(x->getPos()+fspeed);
    x->setSelfPush(zero2<float>());
  }
  sf::Vector2f fs = levelSlot.validMove(player.getPos(), player.getSize(), player.getSelfPush());
  player.setPos(player.getPos() + fs);
  std::deque<Inter> interactions;
  player.setSelfPush(zero2<float>());

  //eventually, this will iterate until a stable configuration is reached.
  for(int i=0;i<1;i++) {
    //create a list of interactions
    //check each pair of objects (plus player)
    for(unsigned i=0;i<levelSlot.getObjNum();i++) {
      if(levelSlot.getObj(i).getStatus() == Object::Inactive || levelSlot.getObj(i).getStatus() == Object::Destroy || levelSlot.getObj(i).held) {
        continue;
      }
      for(unsigned j=i;j<levelSlot.getObjNum();j++) {
        if(i==j || levelSlot.getObj(j).getStatus() == Object::Inactive || levelSlot.getObj(j).getStatus() == Object::Destroy || levelSlot.getObj(i).held) {
          continue;
        }
        //check interaction between objects
        Object* o1 = (levelSlot.getObjPtr(i));
        Object* o2 = (levelSlot.getObjPtr(j));

        if(rect_intersect(o1, o2)) {
          //create and populate an Inter object
          interactions.push_back(Inter(o1, o2));
        }
      }
      //check interaction with player and object[i];
      Object* ob = (levelSlot.getObjPtr(i));
      if(rect_intersect(ob, &player)) {
        //interaction
        interactions.push_back(Inter(ob, &player));
      }
    }


    //sort objects by priority

    //calculate interaction ranking
    for(auto& x : interactions) {
      x.calculatePriority();
    }

    //reorder interaction vector:
    //  sorting by subpriority then priority with a stable sort ensures that the vector
    //  is lexicographically sorted

    std::stable_sort(interactions.begin(), interactions.end(), [](Inter o, Inter p) {
        return o.subpriority < p.subpriority;
    });

    std::stable_sort(interactions.begin(), interactions.end(), [](Inter o, Inter p) {
        return o.priority < p.priority;
    });


    //redo all this:

    //handle in priority rank
    for(size_t i=0;i<interactions.size();i++) {
      auto& x = interactions[i];

      //check if interaction was already resolved
      if(!rect_intersect(x.o1, x.o2)) {
        continue;
      }

      bool ignore = false; //if true, ignore collision

      //we determine whether to ignore the objects, based on their types

      if(x.o1->Type() == Object::Intangible || x.o2->Type() == Object::Intangible) {
        ignore = true;
      }

      //skip any ignored interactions

      //now that flags have been determined, run handler
      if(!ignore) {
        if(x.o1->Shape() == Object::Rect && x.o2->Shape() == Object::Rect) {
          Results r = rectangle_collide(x.o2, x.o1);

          //handle the collision using the results;

          //r.first is o1 offset
          //r.second is o2 offset
          x.o1->setPos(x.o1->getPos()+r.first);
          x.o2->setPos(x.o2->getPos()+r.second);
          //check the states of the objects
        }
      }

      //end redo all this block

      //call the interaction handler to handle non-position things
      Interface res = x.o1->interact(x.o2, &levelSlot.field, &switchHandler);
      res = res + (x.o2->interact(x.o1, &levelSlot.field, &switchHandler));
      for(auto y : res.message) {
        if(y != "") {
          message.addMessage(y);
        }
      }
      for(auto y : res.cutscene) {
        if(y != "") {
          cutscenePlayer.playCutscene(y);
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
      if(res.camera.size() > 0) {
        /*
        AnimDesc an;
        an.duration = 10;
        an.configName = res.camera;
        camera.startAnimation(an);
        */
        camera.selectConfig(res.camera);
      }
      if(res.menu.size() > 0) {
        //switch to menu
        mainMenu.loadTemplate(res.menu);
      }
    }


  }

}

void MapData::event2Handle() {  //this is the cutscene mode
  if(cutscenePlayer.updateCutscene()) {
    //I'm not sure if anything needs to go here
  }
  else {
    //cutscene is over; do things now... 
    modeSwitcher.setMode(1); //switch back to gameplay mode
  }
  player.update();
}


void MapData::event3Handle() {  //debug mode logic
  return; //no logic occurs
}
void MapData::event4Handle() {  //level select mode logic
  return; //no logic occurs
}

void MapData::event5Handle() {  //world map mode logic
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
