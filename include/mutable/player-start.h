#ifndef PLAYER_START_H
#define PLAYER_START_H

#include "mutable/object.h"

//! Player start position indicator. If multiple exist, then all but first are ignored
/*!
 *  Player's position and size will be taken from this obj's pos,scale,basesize
 *  Used during level load, then ignored.
 *
 *  Arg0 - player speed
 *  Arg1 - player max health
 *  Arg2 - player max act cooldown
 *  Arg3 - player max dmg cooldown
 *
 *  Text - player sprite root name
 *
 */
class PlayerStart : public Object {
public:
  virtual std::string Name() const {return "player_start";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(16,16);}
  virtual bool verify() const;
  PlayerStart(int uid);
};



#endif
