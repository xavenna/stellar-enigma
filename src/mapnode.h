#ifndef MAPNODE_H
#define MAPNODE_H

#include <SFML/Graphics.hpp>
#include <string>
#include "direction.h"

//! A class that represents a single node of the map
/*!
 *  Extends sf::Sprite
 */
class MapNode : public sf::Sprite {
private:
  int id;  //!< The identifier of which tile exists here
  std::string cutname;  //!< The name of a cutscene to trigger. \0 if no cutscene
  DirectionalBool solid; //!< whether the node is solid from each direction
  
public:
  //! Gets the id of the node
  int getId() const;
  //! Sets the id of the node
  void setId(const int&);
  //! sets solidity in one direction
  bool getSolid(Direction) const;
  //! Get the cutname
  std::string getCutname() const;
  //! Gets solidity in one direction
  void setSolid(Direction, bool);
  //! Set the cutname
  void setCutname(const std::string&);
  //! The basic constructor. Sets id to 0 and area position to (0,0)
  MapNode();
  //! Constructs the MapNode with passed attributes
  MapNode(int n, DirectionalBool b, std::string cutn);
};

#endif
