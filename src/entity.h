#ifndef ENTITY_H
#define ENTITY_H

#include "mutable.h"

//! A class for entities capable of moving around. Extends mutable
/*! 
 * Extends Mutable with additional attributes specific to entities. 
 * Used for things such as NPCs and Enemies
 */
class Entity : public Mutable {
protected:
  int type;  //!< The type of entity
  int speed;  //!< How many pixels the entity can move each frame
public:
  //! Gets the type of the entity
  int getType() const;
  //! Gets the speed of the entity
  int getSpeed() const;
  //! Sets the speed of the entity
  void setType(const int&);
  //! Sets the type of the entity
  void setSpeed(int);
  
};
#endif
