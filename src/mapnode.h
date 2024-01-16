#ifndef MAPNODE_H
#define MAPNODE_H

#include <SFML/Graphics.hpp>
#include <string>
#include "direction.h"

//! The base class for map nodes
class NodeBase {
private:
  unsigned id;  //!< The identifier of which tile exists here
  std::string cutname;  //!< The name of a cutscene to trigger. \0 if no cutscene
  DirectionalBool solid; //!< whether the node is solid from each direction
public:
  //! Gets the id of the node
  unsigned getId() const;
  //! Sets the id of the node
  void setId(unsigned);
  //! sets solidity in one direction
  bool getSolid(Direction) const;
  //! Get the cutname
  std::string getCutname() const;
  //! Gets solidity in one direction
  void setSolid(Direction, bool);
  //! Set the cutname
  void setCutname(const std::string&);
  //! The basic constructor. Sets id to 0 and area position to (0,0)
  NodeBase();
  //! Constructs the MapNode with passed attributes
  NodeBase(unsigned n, DirectionalBool b, const std::string& cutn);

};
//! A drawable MapNode
/*!
 *  Extends sf::Sprite
 */
class MapNode : public NodeBase, public sf::Sprite {
private:
  
public:
  //! Constructs the MapNode with passed attributes
  MapNode(unsigned n, DirectionalBool b, const std::string& cutn);
  MapNode();
};

#endif
