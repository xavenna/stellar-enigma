#ifndef MAPNODE_H
#define MAPNODE_H

#include <SFML/Graphics.hpp>
#include <string>

//! A class that represents a single node of the map
/*!
 *
 */
class MapNode {
private:
  int id;  //!< The identifier of which tile exists here
  std::string cutname;  //!< The name of a cutscene to trigger. \0 if no cutscene
public:
  //! Gets the id of the node
  int getId() const;
  //! Sets the id of the node
  void setId(const int&);
  //! Get the cutname
  std::string getCutname();
  //! Set the cutname
  void setCutname(const std::string&);
  //! The sprite that can be drawn to the map
  /*! 
   *  This isn't initialized with a texture, as textures are assigned during
   *  Program execution, right before display drawing.
   */
  sf::Sprite area;
  //! The basic constructor. Sets id to 0 and area position to (0,0)
  MapNode();
  MapNode(int n, std::string cutn);
};

#endif
