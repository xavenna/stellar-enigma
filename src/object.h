#ifndef OBJECT_H
#define OBJECT_H

#include "mutable.h"

//! A class for objects the player can interact with, extends Mutable
/*!
 *  Extends mutable with additional attributes specific to objects
 */
class Object : public Mutable {
protected:
  int id; //!< the internal id of the object
  bool collectable; //!< Whether the object can be collected by the player
public:
  //! get the id of the object 
  int getId() const;
  //! get the collectability of the object
  bool getCollectable() const;
  //! set the id of the object
  void setId(const int&);
  //! set the collectability of the object
  void setCollectable(const bool&);
};

#endif
