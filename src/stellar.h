#ifndef STELLAR_H
#define STELLAR_H
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <deque>
#include <SFML/Graphics.hpp>
#include "util.h"

//this is the primary header file for the stellar enigma engine
//It contains definitions of integral classes, as well as other useful functions


class TextureMap {
  //contains texture mappings for use when assigning textures to sprites
public:
  std::vector<sf::Texture> mapping;
  sf::Texture& getTexture(const int&);
  void assign(int, sf::Texture);
  bool initialize(const std::string&);
  int size() const;
};

class Message {  //this class manages the message slot
private:
  std::string message;
  size_t pointer;  //maybe this should be an int, or an integer
  const size_t width;
  bool printed;  //is message done printing
  const size_t maxCool;  //how many frames of cooldown before more can print
  size_t cooldown;
  std::deque<std::string> printQueue;
public:
  bool getPrinted() const;
  void addMessage(const std::string&);
  void handleMessages();
  Message(size_t, size_t);  // width, maxcool
};

class Mutable {  //this class is the base class for things that can move around
protected:
  int xpos;
  int ypos;
  int width;
  int height;
  bool solid;
public:
  int getXPos() const;
  int getYPos() const;
  int getWidth() const;
  int getHeight() const;
  bool getSolid() const;
  void setXPos(const int&);
  void setYPos(const int&);
  void setWidth(const int&);
  void setHeight(const int&);
  void setSolid(const bool&);
  sf::Sprite area;
};
class Object : public Mutable {  //this class is for objects the player can interact with
protected:
  int id;
  bool collectable;
public:
  int getId() const;
  bool getCollectable() const;
  void setId(const int&);
  void setCollectable(const bool&);
};

class Entity : public Mutable {  //this class is for things such as enemies and npcs
protected:
  int type;
  int speed;
public:
  void setType(const int&);
  int getType() const;
  int getSpeed() const;
  void setSpeed(int);
  
};


class Player : public Mutable {
protected:
  sf::Texture picture;
  int speed;
  int facingDir;
public:
  void initialize(); 
  void update();
  void setSpeed(int);
  void setFacing(int);  // 0=up, clockwise
  int getSpeed() const;
  int getFacing() const;
};

class MapNode {  //this class holds a node on the map. It will use a grid-based system
private:
  int id;  //which thing goes here
public:
  int getId() const;
  void setId(const int&);
  sf::Sprite area;  //this isn't initialized, as textures are updated every frame conditionally
  MapNode();
};

class Level {  //this class holds a complete level. It will encapsulate all things
private:
  std::vector<std::vector<MapNode>> mapBase;
  std::vector<Object> spriteList;
  std::vector<Entity> entityList;
  int tilesizeX;
  int tilesizeY;
public:
  MapNode getNode(const int&, const int&) const;
  void updateNode(const int&, const int&, const MapNode&);
  void assignTextureToNode(const int&, const int&, TextureMap&);
  int loadLevel(const std::string&);
  void loadMutables(const std::string&);
  int getHeight() const;
  int getWidth() const;
  int getTilesizeX() const;
  int getTilesizeY() const;
  Level(const size_t&, const size_t&);
  Level();
};

class CutscenePlayer {
public:
  void PlayCutscene();
};

class Menu {
public:
  sf::Sprite splash;
  sf::Texture spT;

};
bool nodify(std::string&, MapNode&);
bool strToNode(const std::string&, MapNode&);





#endif
