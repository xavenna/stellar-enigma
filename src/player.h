#ifndef PLAYER_H
#define PLAYER_H

#include "mutable.h"
#include "direction.h"

//! Class used for player
/*! *  A special extension of mutable that works specifically for player.
 */
class Player : public Mutable {
protected:
  sf::Texture picture; //!< I don't remember what this is used for
  int speed; //!< How many pixels the player can move per frame
  Direction facingDir; //!< Which direction the player is facing
public:
  //! Initialized the player, This should be rewritten into a constructor
  void initialize();  //this should be made into a constructor
  //! Updates the player's sprite based on position
  /*!
   *  Sets the sprite's position based on where the player is located
   */
  void update(int, int);
  //! Gets the speed of the player
  int getSpeed() const;
  //! Gets the direction of the player
  Direction getFacing() const;
  //! Sets the speed of the player 
  void setSpeed(int);
  //! Sets the direction of the player
  void setFacing(Direction);
  //! Converts the player's position in pixels to position based on tile width
  /*!
   *  Specifically, returns the x position of the tile that the center of the
   *  player lies on
   */
  int getLevelXPos(int tileWidth);
  //! Converts the player's position in pixels to position based on tile height
  /*!
   *  Specifically, returns the y position of the tile that the center of the
   *  player lies on
   */
  int getLevelYPos(int tileHeight);
};

#endif
