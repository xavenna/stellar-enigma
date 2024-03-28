#ifndef PLAYER_H
#define PLAYER_H

#include "misc/effect.h"
#include "mutable/object.h"
#include "misc/direction.h"
#include "utility/texture-cache.h"

//! Class used for player
/*!
 *  A special extension of mutable that works specifically for player.
 *  Can directly respond to keyboard inputs
 */
class Player : public Object {
public:
  //! Contains values for player's interaction status
  enum Status {
    Interacting,  //!< Resolving an Interaction with another object
    Acting,  //!< Entered when action button is pressed
    Invulnerable, //!< Immune to Damage
    Damaged,  //!< Currently recovering from damage
    Normal,   //!< Not in any special state
    Pushed   //!< The player just finished being pushed back by a solid object
  };
  virtual std::string Name() const {return "player";}
  virtual Object::MotionType Type() const {return Object::Play;} 
  bool damaged=false; //!< Has the player been damaged this frame?
  //! Updates the player's sprite based on position
  /*!
   *  Sets the sprite's position based on where the player is located. Ensures the player's position
   *  is in the correct place relative to the currently displayed section of the level
   */
  void update(sf::Vector2i);
  //! Assign texture from cache
  /*!
   *  This needs to be restructured to match Object::draw();
   */
  void assignTexture(TextureCache& cache);
  //! clears all effects
  void clearEffects();
  //! Apply specified effect to player
  void applyEffect(const Effect& e);
  //! Gets the speed of the player
  int getSpeed() const;
  //! Gets the raw speed of the player
  int getRawSpeed() const;
  //! Gets the health of the player
  unsigned getHealth() const;
  //! Gets the raw health of the player
  int getRawHealth() const;
  //! Gets the player score
  unsigned getScore() const;
  //! Gets the direction of the player
  Direction getFacing() const;
  //! Sets the speed of the player 
  void setSpeed(int);
  //! Sets the health of the player 
  void setHealth(unsigned);
  //! Sets the maximum cooldown
  void setMaxCooldown(unsigned);
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
  //! Increments score by specified amount, returns new value
  unsigned modifyScore(int);
  //! Converts the player's position in pixels to tile position
  /*!
   *  Returns the map coordinates of the tile that the center of the player lies on.
   */
  sf::Vector2i getLevelPos(sf::Vector2i tileSize);
  //! constructs player
  /*!
   *  
   */
  Player();
protected:
  int speed; //!< How many pixels the player can move per frame
  unsigned maxCooldown=0; //!< Number of invincibility frames
  unsigned cooldown=0; //!< Remaining invincibility frames
  unsigned health=0; //!< The player's health
  unsigned score=0;  //!< The player's score
  Direction facingDir; //!< Which direction the player is facing
  Player::Status status; //!< Player's current status
  std::vector<Effect> effects; //!< All active effects on the player
};

#endif
