#ifndef FIELD_H
#define FIELD_H
#include <fstream>
#include <iostream>
#include <json11.hpp>
#include "level/mapnode.h"
#include "misc/util.h"
#include "misc/vect.h"


//! A utility class for storing walls
class Wall {
public:
  sf::Vector2f p1; //!< first point
  sf::Vector2f p2; //!< second point

  //methods
  sf::Vector2f push() const; //!< Returns the wall's push vector
  sf::Vector2f face() const; //!< Returns wall's direction vector
  Wall(sf::Vector2f, sf::Vector2f);

};


class Field {
private:
  std::vector<std::vector<NodeBase>> mapBase; //!< The level data of the map
  sf::Vector2u tilesize;
  std::vector<Wall> walls; //the list of walls found in the level
public:
  //! returns a copy of specified node
  NodeBase getNode(unsigned, unsigned) const;
  //! overwrites the mapnode at position x,y with node
  void updateNode(unsigned x, unsigned y, const NodeBase& node);
  //! Gets map height
  unsigned getHeight() const;
  //! Gets map width
  unsigned getWidth() const;

  //! gets tile width
  unsigned getTilesizeX() const;
  //! gets tile height
  unsigned getTilesizeY() const;
  //! gets tilesize as a sf::Vector2i
  sf::Vector2u getTilesize() const;

  //! Constructs a field with specified size
  Field(size_t, size_t);

  //! Loads mapBase from file, specified by argument.
  /*! 
   *  Searches for file in /levels/
   */
  int loadLevel(const std::string&);
  //! Loads tiles from a json file generated by OGMO editor
  int loadJsonLevel(const std::string& levelname);
  //! sets up walls variable
  bool initializeWalls();

  //! Determines how far object can move before colliding with a wall
  sf::Vector2f wallHandle(const sf::Vector2f pos, const sf::Vector2f size, const sf::Vector2f speed) const;

  //! Determines t2 for the specified scenario
  float findClosestPointInRange(const sf::Vector2f w0, const sf::Vector2f w1, const sf::Vector2f w2, const sf::Vector2f r0, const sf::Vector2f r, const sf::Vector2f p) const;
};

//! parses a string containing a node, uses nodify
bool nodify(std::string&, MapNode&);
//! creates a node from a string representation of a node
bool strToNode(const std::string&, MapNode&);


struct Edge {
  unsigned x; //!< x-coord of the node associated
  unsigned y; //!< y-coord of the node associated
  Direction d; //!< Which side of specified node the edge is on.
};

#endif
