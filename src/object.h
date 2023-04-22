#ifndef OBJECT_H
#define OBJECT_H

// TODO: redesign the object system
#include "mutable.h"
#include "player.h"
#include "field.h"
#include "texture-cache.h"

#include <array>
#include <tuple>


//! A utility class for object behavior function return values
class Interface {
public:
  sf::Vector2i pos;
  std::string message;
  std::string cutscene;
  Interface(sf::Vector2i pos, std::string mes, std::string cut);
  Interface();
};

//! A class for objects, anything with dynamic behavior
/*!
 *  Extends mutable with additional attributes specific to objects
 *  Any derived classes should just redefine the virtual functions, without adding new
 *  members
 *  (for objects not interacting with player, don't redefine behave. For invisible
 *   objects, don't redefine draw)
 *
 *  To set status, use bitmasking to toggle specified bit.
 */
class Object : public Mutable {
public:
  enum Status {
    Normal, //!< No special conditions
    Destroy //!< Object should be destroyed
  };
  static const int Dest = 0x1;
  //! base version of behave();
  virtual Interface behave(Player*, Field*, bool);
  //! produces the proper CacheNodeAttributes for the state of the object
  virtual CacheNodeAttributes draw(const TextureCache*);
  //! get the id of the object 
  int getId() const;
  //! get the value of the object 
  int getValue() const;
  //! get the text
  std::string getText() const;
  //! get all args
  std::array<int, 8> getArgs() const;
  //! get specified arg
  int getArg(std::size_t) const;
  //! get the obj's status
  int getStatus() const;
  //! set the id of the object
  void setId(int);
  //! set the value of the object
  void setValue(int);
  //! set the text of the object
  void setText(const std::string&);
  //! get value of specified arg
  void setArg(std::size_t slot, int value);
  //! full parametrized constructor
  Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  //! ObjArg-less parametrized constructor (for legacy reasons)
  Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt);
  //! empty constructor
  Object();
protected:
  int id; //!< the internal id of the object
  int value; //!< another int field that can be used
  std::string text; //!< A text field that can be used by the object
  std::array<int, 8> args; //!< Arguments for each object, usage varies
  //yes, I did just steal SMG's obj_arg system, I couldn't think of a better way

  int status; //!< Internal status, can be externally read but not written
};

//! An object that is solid and blocks movement
class Solid : public Object {
public:
  virtual Interface behave(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  Solid(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Solid();
  Solid(Object);
};
//! An object that can be pushed around, sokoban style
class Pushable : public Object {
public:
  virtual Interface behave(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  Pushable(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Pushable();
  Pushable(Object);
};
//! An object that can be picked up, plays cutscene 'key' when picked up
class Key : public Object {
public:
  virtual Interface behave(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  Key(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Key();
  Key(Object);
};
//! An object that displays a message when picked up
class Board : public Object {
public:
  virtual Interface behave(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  Board(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Board();
  Board(Object);
};
//! An object that damages player on contact and blocks movement
class Spike : public Object {
public:
  virtual Interface behave(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  Spike(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  Spike();
  Spike(Object);
};
//! An object that plays a cutscene upon contact
class CutPlay : public Object {
public:
  virtual Interface behave(Player*, Field*, bool);
  CutPlay(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  CutPlay();
  CutPlay(Object);
};

//! An object that toggles between two states upon contact
class ToggleBlock : public Object {
public:
  virtual Interface behave(Player*, Field*, bool);
  virtual CacheNodeAttributes draw(const TextureCache*);
  ToggleBlock(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);
  ToggleBlock();
  ToggleBlock(Object);
};



//! A class for holding the object list, as well as serving as an Object Factory
/*!
 *  Holds a list of pointers to objects. The pointers are dynamically allocated, and
 *  freed by the destructor. Objects can be added as empty, by copy, or by traits
 *  Objects can be returned by value, by pointer, or by reference, depending on situation
 */
class ObjContainer {
public:
  //! Object type, used to create the appropriate type of object
  enum Type {
    obj, //!< Basic object
    solid, //!< Solid object
    pushable, //!< Pushable object
    key, //!< Key
    board, //!< Message Board
    cutscene_player, //!< Cutscene Player
    spike, //!< Damaging Object
    toggle_block //!<two-state block, toggles upon player contact
  };
  //! Get reference to specified object
  Object& getObjRef(unsigned);
  //! Get pointer to specified object
  Object* getObjPtr(unsigned);
  //! Get copy of specified object
  Object getObj(unsigned) const;
  //! Create Object of specified type using arguments
  bool storeObj(sf::Vector2i, sf::Vector2i, int, int, bool, const std::string&, std::array<int, 8>, Type);
  //! Create empty Object of specified type
  bool storeObj(Type);
  //! Create copy of passed object, as specified type
  bool storeObj(Object ob, Type);

  //! Remove specified object and free its allocated memory
  void removeObj(unsigned);

  //! Get the number of objects stored
  std::size_t size() const;
  //! Constructs object container
  ObjContainer();
  //! Destructor; Frees all object slots
  ~ObjContainer();
private:
  //! where pointers are stored internally; ObjContainer owns the objects
  std::deque<Object*> list;
};

//! Utility function to transform an id to a type
ObjContainer::Type getType(unsigned p);


#endif
