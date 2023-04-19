#ifndef OBJECT_H
#define OBJECT_H

// TODO: redesign the object system
#include "mutable.h"
#include "player.h"
#include "field.h"
#include "message.h"

#include <array>
#include <tuple>



//! A class for objects the player can interact with, extends Mutable
/*!
 *  Extends mutable with additional attributes specific to objects
 *  Any derived classes should just redefine the virtual functions, without adding new
 *  members
 */
class Object : public Mutable {
protected:
  int id; //!< the internal id of the object
  int value; //!< another int field that can be used
  bool collectable; //!< Whether the object can be collected by the player
  std::string text; //!< A text field that can be used by the object
  std::array<int, 8> args; //!< Arguments for each object, usage varies by type
  //yes, I did just steal SMG's obj_arg system, I couldn't think of a better way
public:
  //! base version of behave();
  virtual std::tuple<sf::Vector2i, std::string, std::string> behave(Player*, Field*, bool);
  //! get the id of the object 
  int getId() const;
  //! get the value of the object 
  int getValue() const;
  //! get the collectability of the object
  bool getCollectable() const;
  //! get the text
  std::string getText() const;
  //! get all args
  std::array<int, 8> getArgs() const;
  //! get specified arg
  int getArg(std::size_t) const;
  //! set the id of the object
  void setId(int);
  //! set the value of the object
  void setValue(int);
  //! set the collectability of the object
  void setCollectable(bool);
  //! set the text of the object
  void setText(const std::string&);
  //! get value of specified arg
  void setArg(std::size_t slot, int value);
  //! full parametrized constructor
  Object(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8>);
  //! ObjArg-less parametrized constructor (for legacy reasons)
  Object(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt);
  //! empty constructor
  Object();
  //! rvalue constructor
};

class Stone : public Object {
public:
  virtual std::tuple<sf::Vector2i, std::string, std::string> behave(Player*, Field*, bool);
  Stone(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8>);
  Stone();
  Stone(Object);
};
class Crate : public Object {
public:
  virtual std::tuple<sf::Vector2i, std::string, std::string> behave(Player*, Field*, bool);
  Crate(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8>);
  Crate();
  Crate(Object);
};
class Key : public Object {
public:
  virtual std::tuple<sf::Vector2i, std::string, std::string> behave(Player*, Field*, bool);
  Key(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8>);
  Key();
  Key(Object);
};
class Board : public Object {
public:
  virtual std::tuple<sf::Vector2i, std::string, std::string> behave(Player*, Field*, bool);
  Board(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8>);
  Board();
  Board(Object);
};
class Spike : public Object {
public:
  virtual std::tuple<sf::Vector2i, std::string, std::string> behave(Player*, Field*, bool);
  Spike(int x, int y, int wid, int hei, int i, int v, bool sol, bool col, const std::string& txt, std::array<int, 8>);
  Spike();
  Spike(Object);
};


//! A class for holding all objects, as well as serving as an Object Factory
/*!
 *  As objects are now polymorphic, with different classes for each type, another approach
 *  was needed. The ObjContainer allocates memory for each object, and serves as a factory for 
 *  objects
 */
class ObjContainer {
public:
  enum Type {
    obj,
    stone,
    crate,
    key,
    board,
    spike
  };
  Object& getObjRef(unsigned);
  Object* getObjPtr(unsigned);
  Object getObj(unsigned) const;
  bool storeObj(sf::Vector2i, sf::Vector2i, int, int, bool, bool, const std::string&, std::array<int, 8>, Type);
  bool storeObj(Type);
  bool storeObj(Object ob, Type);

  void removeObj(unsigned);

  std::size_t size() const;
  ObjContainer();
  ~ObjContainer();
private:
  //! where pointers are stored internally; ObjContainer owns the objects
  std::deque<Object*> list;
};

ObjContainer::Type getType(unsigned p);


#endif
