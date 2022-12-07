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
  unsigned maxCooldown=0; //!< Number of invincibility frames
  unsigned cooldown=0; //!< Remaining invincibility frames
  unsigned health=0; //!< The player's health
  Direction facingDir; //!< Which direction the player is facing
public:
  bool damaged=false; //!< Has the player been damaged this frame?
  //! Initialized the player, This should be rewritten into a constructor
  void initialize(unsigned);  //this should be made into a constructor
  //! Updates the player's sprite based on position
  /*!
   *  Sets the sprite's position based on where the player is located
   */
  void update(sf::Vector2i);
  //! Gets the speed of the player
  int getSpeed() const;
  //! Gets the health of the player
  int getHealth() const;
  //! Gets the direction of the player
  Direction getFacing() const;
  //! Sets the speed of the player 
  void setSpeed(int);
  //! Sets the health of the player 
  void setHealth(int);
  //! Sets the direction of the player
  void setFacing(Direction);
  //! Decrements cooldown and returns the new cooldown
  unsigned decrementCooldown();
  //! Get current cooldown
  unsigned getCooldown();
  //! Reset cooldown to maximum
  void resetCooldown();
  //! Decrements health and returns the new health
  unsigned modifyHealth(int);
  //! Get current health
  unsigned getHealth();
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
  //! constructs player
  /*!
   *  
   */
  Player();
};

#endif
