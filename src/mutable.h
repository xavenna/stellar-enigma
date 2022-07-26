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
  int xpos; //!< X position of the mutable
  int ypos; //!< Y position of the mutable
  int xScreen; //!< Which screen mutable is on horizontally
  int yScreen; //!< Which screen mutable is on vertically
  int width; //!< width of the mutable (I forget what the units are)
  int height; //!< height of the mutable
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
