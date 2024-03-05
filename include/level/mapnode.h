#ifndef MAPNODE_H
#define MAPNODE_H

#include <SFML/Graphics.hpp>
#include <string>
#include "misc/direction.h"

//! The base class for map nodes
class NodeBase {
private:
  unsigned id;  //!< The identifier of which tile exists here
  //! Specifies which tile to use
  /*!
   *  Using the tileset's grid, get the tile at position (x,y).
   */
  sf::Vector2i tile;
  //! Specifies which tileset to use
  int tileset;
  DirectionalBool solid; //!< whether the node is solid from each direction
public:
  //! Gets the id of the node
  unsigned getTileset() const;
  //! Sets the id of the node
  void setTileset(unsigned);
  //! Gets the tile pos of the node
  sf::Vector2i getTile() const;
  //! Sets the tile pos of the node
  void setTile(sf::Vector2i);
  //! sets solidity in one direction
  bool getSolid(Direction) const;
  //! Gets solidity in one direction
  void setSolid(Direction, bool);
  //! The basic constructor. Sets id to 0 and area position to (0,0)
  NodeBase();
  //! Constructs the MapNode with passed attributes
  NodeBase(unsigned n, DirectionalBool b);

};
//! A drawable MapNode
/*!
 *  Extends sf::Sprite
 */
class MapNode : public NodeBase, public sf::Sprite {
private:
  
public:
  //! Constructs the MapNode with passed attributes
  MapNode(unsigned n, DirectionalBool b);
  MapNode();
};

#endif
