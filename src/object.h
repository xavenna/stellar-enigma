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
  int value; //!< another int field that can be used
  bool collectable; //!< Whether the object can be collected by the player
  std::string text; //!< A text field that can be used by the object
public:
  //! get the id of the object 
  int getId() const;
  //! get the value of the object 
  int getValue() const;
  //! get the collectability of the object
  bool getCollectable() const;
  //! get the text
  std::string getText() const;
  //! set the id of the object
  void setId(int);
  //! set the value of the object
  void setValue(int);
  //! set the collectability of the object
  void setCollectable(bool);
  //! set the text of the object
  void setText(const std::string&);
  //! full parametrized constructor
  Object(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt);
  //! empty constructor
  Object();
};

#endif
