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



class TextureMap {
  //contains texture mappings for use when assigning textures to sprites
public:
  std::vector<sf::Texture> mapping;
  sf::Texture& getTexture(const int&);
  void assign(int, sf::Texture);
  bool initialize(const std::string&);
  int size();
};

class Mutable {  //this class is the base class for things that can move around
protected:
  int xpos;
  int ypos;
  int width;
  int height;
  bool solid;
public:
  int getXPos();
  int getYPos();
  int getWidth();
  int getHeight();
  bool getSolid();
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
  int getId();
  int getCollectable;
  void setId(const int&);
  void setCollectable(const int&);
};

class Entity : public Mutable {  //this class is for things such as enemies and npcs
protected:
  int type;
  int speed;
public:
  void setType(const int&);
  int getType();
  int getSpeed();
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
  int getSpeed();
  void setSpeed(int);
  int getFacing();
  void setFacing(int);  // 0=up, clockwise
};

class MapNode {  //this class holds a node on the map. It will use a grid-based system
private:
  int id;  //which thing goes here
public:
  int getId();
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
  MapNode getNode(const int&, const int&);
  void updateNode(const int&, const int&, const MapNode&);
  void assignTextureToNode(const int&, const int&, TextureMap&);
  int loadLevel(const std::string&);
  void loadMutables(const std::string&);
  int getHeight();
  int getWidth();
  int getTilesizeX();
  int getTilesizeY();
  Level(const int&, const int&);
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
bool strToNode(std::string&, MapNode&);





#endif
