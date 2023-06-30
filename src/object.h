#ifndef OBJECT_H
#define OBJECT_H

// TODO: redesign the object system
#include "mutable.h"
#include "player.h"
#include "field.h"
#include "texture-cache.h"
#include "vect.h"

#include <array>
#include <tuple>

class Object;

//! A utility class for object behavior function return values
/*!
 *  This serves as an API allowing the virtual functions to cause effects, such as playing
 *  cutscenes, displaying messages, etc. Originally, a std::tuple was used, but this
 *  method is more flexible
 */
class Interface {
public:
  sf::Vector2i pos;  //!< Position of the object after interaction is complete
  std::string message;  //!< Message to display
  std::string cutscene;  //!< Cutscene to play
  std::vector<Object> objs; //!< Objects to create
  Interface(sf::Vector2i pos, std::string mes, std::string cut);  //!< Value Constructor
  Interface(sf::Vector2i pos, std::string mes, std::string cut, std::vector<Object> o);  //!< Value Constructor, ft. Objects to create
  Interface();  //!< Empty Constructor
};

//! A class for objects, anything with dynamic behavior
/*!
 *  Extends mutable with additional attributes specific to objects
 *  Any derived classes should just redefine the virtual functions, without adding new
 *  members
 * 
 *  Virtual functions:
 *  behave - called every frame, normal behavior occurs here
 *  interact - called when something interacts with the object
 *  draw - tells the engine how to draw the object
 *  
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
  //! base version of interact();
  virtual Interface interact(Player*, Field*, bool);
  //! produces the proper CacheNodeAttributes for the state of the object
  virtual CacheNodeAttributes draw(const TextureCache*);
  //! base version of behave();
  virtual Interface behave();
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
  //! set all args
  void setArgs(std::array<int, 8>);
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
  std::array<int, 4> vars; //!< Internal variables, used similarly to args

  int status; //!< Internal status, can be externally read but not written
};

//utility functions for objects
int binCat(std::uint16_t, std::uint16_t);
Vector2<std::uint16_t> binDecat(int);

#endif
