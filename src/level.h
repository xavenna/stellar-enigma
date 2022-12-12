#ifndef LEVEL_H
#define LEVEL_H

#define WINDOW_WIDTH 8
#define WINDOW_HEIGHT 8

#include "mapnode.h"
#include "object.h"
#include "entity.h"
#include "texturemap.h"
#include "player.h"
#include <iostream>
#include <fstream>
#include "util.h"

//! The class that holds a complete level
/*!
 *  Contains all tiles, as well as objects and entities.
 */
class Level {
private:
  std::vector<std::vector<MapNode>> mapBase; //!< The level data of the map
  std::vector<Object> objectList; //!< A list of objects found in the map
  std::vector<Entity> entityList; //!< A list of entities found in the map
  int tilesizeX;  //!< The width of a tile, in pixels? currently unused?
  int tilesizeY;  //!< The height of a tile, in pixels? currently unused?
  int winOffX = 0;
  int winOffY = 0;
public:
  //! returns a copy of specified node
  MapNode getNode(const int&, const int&) const;
  //! returns a copy of specified object
  Object getObj(int index) const;
  //! returns a copy of specified entity
  Entity getEnt(int index) const;
  //! updates specified object with passed object
  void updateObj(unsigned index, const Object&);
  //! a fixed-size map that is actually drawn to the screen
  std::array<std::array<MapNode, WINDOW_HEIGHT>, WINDOW_WIDTH> window;
  //! overwrites the mapnode at position x,y with node
  void updateNode(const int& x, const int& y, const MapNode& node);
  //! This will copy the necessary nodes over to window in order to set it up
  void readyWindow(int xScreen, int yScreen);
  //! like readyWindow, but better
  void newReadyWindow(int xscr, int yscr);
  //! sets the positions of the sprites of the window
  void updateWindowPos();
  //! Uses a texturemap to assign a texture to specified node
  void assignTextureToNode(const int&, const int&, TextureMap&);
  //! Uses a texturemap to assign a texture to specified node in window
  void assignTextureToWinNode(const int&, const int&, TextureMap&);
  //! Uses a texturemap to assign a texture to the specified object
  void assignTextureToObject(int, TextureMap&);
  //! Uses a texturemap to assign a texture to the specified entity
  void assignTextureToEntity(int, TextureMap&);
  //! Loads mapBase from file, specified by argument.
  /*! 
   *  Searches for file in /levels/
   */
  int loadLevel(const std::string&);
  //! Loads mutables from file, specified by argument.
  /*! 
   *  Searches for file in /levels/
   */
  void loadMutables(const std::string&);
  //! Gets map height
  int getHeight() const;
  //! Gets map width
  int getWidth() const;
  //! gets tile width
  int getTilesizeX() const;
  //! gets tile height
  int getTilesizeY() const;
  //! gets tilesize as a sf::Vector2i
  sf::Vector2i getTilesize() const;
  //! gets the number of objects
  int getObjNum() const;
  //! gets the number of entities
  int getEntNum() const;

  //! Adds passed entity to entityList
  void addEntity(const Entity& en);
  //! Adds passed Object to objectList
  void addObject(const Object& ob);
  //! Removes specified entity from entity list
  void removeEntity(unsigned index);
  //! Removes specified object from object list
  void removeObject(unsigned index);

  //! Doesn't do anything yet
  void handleEntities();
  //! Handles objects
  /*!
   *  Updates entity tile positions
   */
  void handleObjects(sf::Vector2i pos, sf::Vector2i size);
  //! Tells whether object is on the correct screen to be displayed
  bool displayObject(unsigned index, sf::Vector2i ppos, sf::Vector2i size) const;

  //! Determines how far the player can move before running into an obstacle
  sf::Vector2i validMove(sf::Vector2i pos, sf::Vector2i size, sf::Vector2i speed, int ignore=-1) const;
  //! A constructor that sets the size of mapBase to (arg1, arg2)
  /*!
   *  Doesn't set tilesize
   */
  Level(const size_t&, const size_t&);
  //! The basic constructor, initializes mapBase to size 0
  /*!
   *  Doesn't set tilesize
   */
  Level();
  //! This can be set to true to request a display update
  bool displayUpdate = true;
};

//! parses a string containing a node, uses nodify
bool nodify(std::string&, MapNode&);
//! creates a node from a string representation of a node
bool strToNode(const std::string&, MapNode&);
//! creates an object from a string representation of an object
bool str2obj(const std::string& line, Object& node);

#endif
