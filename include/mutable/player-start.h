#ifndef PLAYER_START_H
#define PLAYER_START_H

#include "mutable/object.h"

//! Player start position indicator. If multiple exist, then all but first are ignored
/*!
 *  
 *
 *
 */
class PlayerStart : public Object {
public:
  virtual std::string Name() const {return "player_start";}
  PlayerStart(int uid);
};



#endif
