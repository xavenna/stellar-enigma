#ifndef ICON_H
#define ICON_H

#include <SFML/Graphics.hpp>
#include "player.h"

//! A class describing icons, components of the panel
/*! 
 *  As it extends sf::Sprite, it is drawable. It contains
 */
class Icon : public sf::Sprite {
public:
  //! Assigns the size of the icon
  void setSize(sf::Vector2u);
  //! Assigns the position of the icon
  void setPos(sf::Vector2i);
  //! Assigns the id of the icon
  void setId(unsigned);
  //! Returns the size of the icon
  sf::Vector2u getSize() const;
  //! Returns the position of the icon
  sf::Vector2i getPos() const;
  //! Returns the id of the icon
  unsigned getId() const;
  //! The update function. Determines which function to use based on the callback attribute
  virtual void update();
  //! Constructs the icon, using the passed string as the callback.
  Icon(std::string);
  //! The texture used for the icon
  sf::Texture tex;
protected:
  sf::Vector2u size;
  sf::Vector2i pos;
  std::string callback; //!< An identifier specifying which function to use for updates
  unsigned id;
};


//! An altternate version of icon that contains a player pointer
class PIcon : public Icon {
public:
  //! A player pointer used for updates
  const Player* player;
  //! Updates the icon, using 'callback' to determine which function to use
  void update(const Player*, TextureCache*);
  //! Constructs the icon, using passed string as 'callback'
  PIcon(std::string);
};


//! An update function that does nothing`
void nullAct();
//! An update function that changes based on the health of the player
void showHealthAsNumber(const Player*, sf::Texture&, TextureCache*);
void showPoints(const Player*, sf::Texture&, TextureCache*);
#endif
