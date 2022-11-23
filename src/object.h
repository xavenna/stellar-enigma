#ifndef OBJECT_H
#define OBJECT_H

#include "mutable.h"

#include "player.h"
//! A class for objects the player can interact with, extends Mutable
/*!
 *  Extends mutable with additional attributes specific to objects
 */
class Object : public Mutable {
protected:
  int id; //!< the internal id of the object
  bool collectable; //!< Whether the object can be collected by the player
  std::string text; //!< A text field that can be used by the object
public:
  //! get the id of the object 
  int getId() const;
  //! get the collectability of the object
  bool getCollectable() const;
  //! get the text
  std::string getText() const;
  //! set the id of the object
  void setId(const int&);
  //! set the collectability of the object
  void setCollectable(const bool&);
  //! set the text of the object
  void setText(const std::string&);
  //! full parametrized constructor
  Object(int x, int y, int wid, int hei, int i, bool sol, bool col, const std::string& txt);
  //! empty constructor
  Object();
};

#endif
