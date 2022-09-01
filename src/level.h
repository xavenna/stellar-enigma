#ifndef LEVEL_H
#define LEVEL_H

#define WINDOW_WIDTH 8
#define WINDOW_HEIGHT 8

#include "mapnode.h"
#include "object.h"
#include "entity.h"
#include "texturemap.h"
#include <iostream>
#include <fstream>
#include "util.h"

//! The class that holds a complete level
/*!
 *  This is supposed to be fully encapsulated
 *  Mutable loading isn't complete yet
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
  //! a fixed-size map that is actually drawn to the screen
  std::array<std::array<MapNode, WINDOW_HEIGHT>, WINDOW_WIDTH> window;
  //! overwrites the mapnode at position x,y with node
  void updateNode(const int& x, const int& y, const MapNode& node);
  //! This will copy the necessary nodes over to window in order to set it up
  void readyWindow(int xScreen, int yScreen);
  //! like readyWindow, but better
  void newReadyWindow(int xpos, int ypos);
  //! sets the positions of the sprites of the window
  void updateWindowPos();
  //! Uses a texturemap to assign a texture to specified node
  void assignTextureToNode(const int&, const int&, TextureMap&);
  //! Uses a texturemap to assign a texture to specified node in window
  void assignTextureToWinNode(const int&, const int&, TextureMap&);
  //! Uses a texturemap to assign a texture to the specified object
  void assignTextureToObject(int, TextureMap&);
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
  //! gets the number of objects
  int getObjNum() const;
  //! gets the number of entities
  int getEntNum() const;

  //make functions for working with objects and entities
  void addEntity(const Entity& en);
  void addObject(const Object& ob);
  void removeEntity(unsigned index);

  void handleEntities();
  void handleObjects();
  bool displayObject(unsigned index);

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
