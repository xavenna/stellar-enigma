#include "stellar-enigma.hpp"
#include "mapdata.h"
#include "color.h"
#include <algorithm>
#include <functional>

void MapData::customInit() {
  //std::cerr << "\x1b[2J";
  //initialize player
  player.setXPos(32);
  player.setYPos(32);
  player.setWidth(16);
  player.setHeight(16);
  player.setSpeed(4);
  player.setHealth(5);
  //initialize some sound things
  sf::SoundBuffer step;
  step.loadFromFile("assets/audio/thud.wav");
  musicPlayer.registerSound("step", step);
  musicPlayer.playMusic("axolotl.ogg");
  //initialize cutscene file
  cutsceneManager.loadCutscenes("default");

  //setup main menu
  mainMenu.spT.loadFromFile("assets/splash/mainmenu.png");
  mainMenu.splash.setTexture(mainMenu.spT);
  modeSwitcher.setMode(0);

  /// diagnostic information
}

void initialSetup(std::string& name, int& framerate) {
  name = "Stellar Enigma Engine Test";  //this is the name of the window
  framerate = 30;   //the framerate
}

int MapData::event0Handle() {  //this mode is used for the main menu
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


  // turn the interaction handler into a seperate function so it can be recursed
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

    /*
    for(auto y : interactions) {
      std::cerr << '[' << y.priority << ',' << y.subpriority << ']';
    }
    */
    //handle in priority rank
    for(size_t i=0;i<interactions.size();i++) {
      bool remove = false;
      auto& x = interactions[i];
      //std::cerr << "\x1b[0K";
      if(x.player1 || x.player2) {
        x.o1->savePos();
        player.savePos();
        //obj-player interaction
        //call the obj's interact function on the player

        Interface res = x.o1->interact(&player, &levelSlot.field, &switchHandler);
        if(res.message != "") {
          message.addMessage(res.message);
        }
        if(res.cutscene != "") {
          if(cutsceneManager.cutsceneExists(res.cutscene)) {
            cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(res.cutscene));
          }
          modeSwitcher.setMode(2);
        }

        for(auto y : res.objs) {
          //create any requested objects (these won't have a link id)
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
          if(res.message != "") {
            message.addMessage(res.message);
          }
          if(res.cutscene != "") {
            if(cutsceneManager.cutsceneExists(res.cutscene)) {
              cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(res.cutscene));
            }
            modeSwitcher.setMode(2);
          }

          for(auto y : res.objs) {
            //create any requested objects
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

    //std::cerr << '\n';
    if(interactions.empty() || n >= 6) {
      // the number is a hardcap for iterations, it may change
      // also break if the list remains identical for several iterations in a row
      // i'm not sure what the best way to do this is...
      break;
    }

  }
  //std::cerr << "\x1b[H";


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

  //levelSlot.handleEntities();
  levelSlot.handleObjects(player.getPos(), player.getSize(), &switchHandler);

  // If player has died, display death screen and switch to mode 0 
  if(player.getHealth() == 0) {
    modeSwitcher.setMode(0);
    mainMenu.spT.loadFromFile("assets/splash/deathscreen.png");
    mainMenu.splash.setTexture(mainMenu.spT);
    mainMenu.onPress = -1;
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
  sf::Vector2i oldps(player.getScreen().x, player.getScreen().y);
  player.update(levelSlot.getTilesize());
  sf::Vector2i newps(player.getScreen().x, player.getScreen().y);

  if(oldps != newps)
    levelSlot.displayUpdate = true;
}


void MapData::event3Handle() {  //debug mode logic
  return; //no logic occurs
}
