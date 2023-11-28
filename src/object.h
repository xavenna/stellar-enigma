#ifndef OBJECT_H
#define OBJECT_H

#include "mutable.h"
#include "player.h"
#include "field.h"
#include "texture-cache.h"
#include "vect.h"
#include "switch-handler.h"

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
  //no longer needed
  std::string message;  //!< Message to display
  std::string cutscene;  //!< Cutscene to play
  std::string sound; //!< Sound to play
  std::vector<Object> objs; //!< Objects to create
  Interface(sf::Vector2i pos, std::string mes, std::string cut);  //!< Value Constructor
  Interface(sf::Vector2i pos, std::string mes, std::string cut, std::vector<Object> o);  //!< Value Constructor, ft. Objects to create
  Interface();  //!< Empty Constructor
};

//! A utility struct for obj-container to pass messages to objects
struct msg {
  char type; //!< What type of message is being sent? Defined on a per-obj basis
  std::array<uint8_t, 4> data; //!< Data fields to use
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
  enum Type {
    Static, //!< Objects that can't be moved
    Sliding, //!< Can be moved by others
    Intangible, //!< Doesn't interact with other objects
    Entity //!< Something that moves by itself
  };
  enum Status {
    Normal, //!< No special conditions
    Destroy //!< Object should be destroyed
  };
  enum SW {  //Correlates switch array positions to names
    Appear = 0, //!< Makes the object appear
    Disappear = 1, //!< Makes the object disappear
    Act1 = 2, //!< Used for activating other objects
    Act2 = 3, //!<
    Trig1 = 4, //!< Other objects trigger these
    Trig2 = 5, //!<
    Gen1 = 6, //!< Currently Unused
    Gen2 = 7  //!< Currently Unused
  };
  //! I have no idea what this is for
  static const int Dest = 0x1;

  //! Object's motion type
  virtual int Type() {return Object::Intangible;} 
  //! object interaction priority
  virtual int priority() {return 32;} //0 is highest, decreases counting up
  //! base version of interact();
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  //! version of interact for object-object interactions
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  //! produces the proper CacheNodeAttributes for the state of the object
  virtual CacheNodeAttributes draw(const TextureCache*);
  //! base version of behave();
  virtual Interface behave(SwitchHandler*);
  //! base version of verify, which ensures object config is valid
  virtual bool verify();
  //! used for the objcontainer to send complex messages to parents (maybe more later)
  virtual void notify(msg);

  //! get the id of the object 
  int getId() const;
  //! get the link id of the object
  int getLinkID() const;
  //! get the parent id of the object
  int getParentID() const;
  //! get the texture id
  int getTextureID() const;
  //! get the unique id of the object
  int getUID() const;
  //! get all args
  std::array<int, 8> getArgs() const;
  //! get specified arg
  int getArg(std::size_t) const;
  //! get the obj's status
  int getStatus() const;
  //! get the text
  std::string getText() const;
  //! set all switches at once
  void setSwitches(std::array<int, 8>);
  //! set the text of the object
  void setText(const std::string&);
  //! set all args at once. Probably unnecessary
  void setArgs(std::array<int, 8>);
  //! set the id of the object
  void setId(int);
  //! set the link id of the object
  void setLinkID(int);
  //! set the parent id of the object
  void setParentID(int);
  //! set the texture id
  void setTextureID(int);
  //! set value of specified arg
  void setArg(std::size_t slot, int v);
  //! get active status of object
  bool getActive() const;
  //! full parametrized constructor, with uid
  Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>, int uid);
  //! empty constructor
  Object();
  //! uid constructor
  Object(int);

  //deprecated

  //! get the value of the object 
  int getValue() const;
  //! set the value of the object
  void setValue(int);
  //! ObjArg-less parametrized constructor
  Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt);
  //! full parametrized constructor
  Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8>);

protected:
  //can be preset
  int link_id; //!< an id used for interactions between objects
  int texture_id; //!< Used to select a texture for the object
  int parent_id; //!< Should contain the Link ID of the parent. Only used for child objs
  int id; //!< the internal id of the object
  std::array<int, 8> args; //!< Arguments for each object, usage varies by object type
  //yes, I did just borrow SMG's obj_arg system, I couldn't think of a better way
  
  std::array<int, 8> switches;

  //internal, aren't preset

  int unique_id; //!< Used internally to differentiate objects
  std::array<int, 4> vars; //!< Internal variables, used similarly to args
  int status; //!< Internal status, can be externally read but not written
  bool active; //!< Whether the object is ready to interact with other objects

  //deprecated
  std::string text; //!< A text field that can be used by the object.
  int value; //!< another int field that can be used.
};

//utility functions for objects
//deprecated
int binCat(std::uint16_t, std::uint16_t);
Vector2<std::uint16_t> binDecat(int);

#endif
