#ifndef FIELD_H
#define FIELD_H
#include <fstream>
#include <iostream>
#include "mapnode.h"
#include "util.h"

class Field {
private:
  std::vector<std::vector<NodeBase>> mapBase; //!< The level data of the map
  sf::Vector2i tilesize;
public:
  //! returns a copy of specified node
  NodeBase getNode(int, int) const;
  //! overwrites the mapnode at position x,y with node
  void updateNode(int x, int y, const NodeBase& node);
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

  Field(size_t, size_t);

  //! Loads mapBase from file, specified by argument.
  /*! 
   *  Searches for file in /levels/
   */
  sf::Vector2i loadLevel(const std::string&);
  sf::Vector2i validMove(sf::Vector2i pos, sf::Vector2i size, sf::Vector2i speed) const;

};

//! parses a string containing a node, uses nodify
bool nodify(std::string&, MapNode&);
//! creates a node from a string representation of a node
bool strToNode(const std::string&, MapNode&);

#endif
