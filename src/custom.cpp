#include "stellar-enigma.hpp"
#include "mapdata.h"
#include "callback.h"
#include "color.h"
#include <tuple>

void MapData::customInit() {
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

  //this might make more sense to be a separate function

  //this structure contains interaction status of all objects
  std::vector<Interaction> objStatus(levelSlot.getObjNum());

  for(int i=0;i<levelSlot.getObjNum();i++) {
    Object ob = levelSlot.getObj(i);
    levelSlot.queryInteractions(player, -1, i);
    //call the interaction detector on levelSlot.getObj(i)
  }


  //for each element of the object/entity vectors, check if it intersects with the player
  //If so, do something...maybe run a callback from the object/entity
  for(int i=0;i<levelSlot.getObjNum();i++) {
    int j = i;
    //check for collision between player and levelSlot.getObject(i);
    Object ob{levelSlot.getObj(i)};
    sf::Vector2i pmin{player.getPos()};
    sf::Vector2i pmax{pmin+player.getSize()-sf::Vector2i(1,1)};

    sf::Vector2i omin{ob.getPos()};
    sf::Vector2i omax{omin+ob.getSize()-sf::Vector2i(1,1)};

    if(pmin.x > omax.x || omin.x > pmax.x || pmin.y > omax.y || omin.y > pmax.y) {
      //no interaction
    }
    else {
      //interaction happens
      //what type of interaction is it?
      //use object id for this
      switch(ob.getId()) {
      case 0:
        //stone: push player out
        solidBehave(ob, this);
        break;
      case 1:
        //crate: pushed by player
        pushBehave(ob, this);
        break;
      case 2:
        //key: picked up, and something happens. Attempts to play the cutscene specified
        //in ob.text. If a valid cutscene is not specified, play cutscene `key'

        if(cutsceneManager.cutsceneExists(ob.getText())) {
          cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(ob.getText()));
        }
        else {
          cutscenePlayer.loadCutscene(cutsceneManager.getCutscene("key"));
        }
        modeSwitcher.setMode(2);
        //destroy object, decrement i;
        levelSlot.removeObject(i);
        i--;
        break;
      case 3:
        //board
        //play the message found in text
        if(ob.getText() != "") {
          message.addMessage(ob.getText());
        }
        break;
      case 4:
        //cutscene player
        if(cutsceneManager.cutsceneExists(ob.getText())) {
          cutscenePlayer.loadCutscene(cutsceneManager.getCutscene(ob.getText()));
          modeSwitcher.setMode(2);
        }
        break;
      case 5:
        //spikes
        hurtBehave(ob, this);
        break;
      case 6:
        //Status effect
        //create effect
        break;
      default:
        //No interaction
        break;
      }
      if(i == j) {
        levelSlot.updateObj(i, ob);
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
  sf::Vector2i oldps(player.getScreen().x, player.getScreen().y);
  player.update(levelSlot.getTilesize());
  sf::Vector2i newps(player.getScreen().x, player.getScreen().y);

  //various updates
  if(oldps != newps)
    levelSlot.displayUpdate = true;

  //levelSlot.handleEntities();
  levelSlot.handleObjects(player.getPos(), player.getSize());

  //cutscenes don't work like this anymore... use a cutscene_player object
  
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
