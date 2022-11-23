#ifndef MUTABLE_H
#define MUTABLE_H

#include <SFML/Graphics.hpp>

//! Base class for things separate from the map, such as enemies and objects
/*!
 *  Contains a sf::Sprite for drawing, as well as base attributes with getters 
 *  and setters
 */
class Mutable {
protected:
  sf::Vector2i pos;
  sf::Vector2i lastPos; //!< Where the mutable was before its most recent move
  sf::Vector2i screen;
  sf::Vector2i size;
  bool solid; //!< whether the mutable can be passed through (I think)
public:
  //! Get the x position of the mutable
  int getXPos() const;
  //! Get the y position of the mutable
  int getYPos() const;
  //! Get the width of the mutable
  int getWidth() const;
  //! get the height of the mutable
  int getHeight() const;
  //! get the x-screen of the mutable
  int getXScreen() const;
  //! get the y-screen of the mutable
  int getYScreen() const;
  //! Get the solidity of the mutable
  bool getSolid() const;

  //! get the position of the mutable as a sf::Vector2i
  sf::Vector2i getPos() const;
  //! get the last position of the mutable as a sf::Vector2i
  sf::Vector2i getLastPos() const;
  //! get the size of the mutable as a sf::Vector2i
  sf::Vector2i getSize() const;
  //! get the screen-pos of the mutable as a sf::Vector2i
  sf::Vector2i getScreen() const;

  //! set the position of the mutable as a sf::Vector2i
  void setPos(sf::Vector2i);
  //! set the last position of the mutable as a sf::Vector2i
  void setLastPos(sf::Vector2i);
  //! set the size of the mutable as a sf::Vector2i
  void setSize(sf::Vector2i);
  //! set the screen-pos of the mutable as a sf::Vector2i
  void setScreen(sf::Vector2i);

  //! set the x position of the mutable
  void setXPos(const int&);
  //! set the y position of the mutable
  void setYPos(const int&);
  //! set both x and y position of the mutable
  void setPos(const int&, const int&);
  //! set the width of the mutable
  void setWidth(const int&);
  //! set the height of the mutable
  void setHeight(const int&);
  //! set the x screen of the mutable
  void setXScreen(const int&);
  //! set the y screen of the mutable
  void setYScreen(const int&);
  //! set the solidity of the mutable
  void setSolid(const bool&);
  //! update the position of the sprite
  sf::Sprite area; //!< the sprite that is drawn to the map
};

#endif
