#ifndef LEVEL_H
#define LEVEL_H

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
  std::vector<Object> spriteList; //!< A list of objects found in the map
  std::vector<Entity> entityList; //!< A list of entities found in the map
  int tilesizeX;  //!< The width of a tile, in pixels? currently unused?
  int tilesizeY;  //!< The height of a tile, in pixels? currently unused?
public:
  //! returns a copy of specified node
  MapNode getNode(const int&, const int&) const;
  //! I'm not sure what this does
  void updateNode(const int&, const int&, const MapNode&);
  //! Uses a texturemap to assign a texture to specified node
  void assignTextureToNode(const int&, const int&, TextureMap&);
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
};

//! parses a string containing a node, uses nodify
bool nodify(std::string&, MapNode&);
//! creates a node from a string representation of a node
bool strToNode(const std::string&, MapNode&);

#endif
