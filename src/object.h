#ifndef OBJECT_H
#define OBJECT_H

#include "mutable.h"
#include "player.h"
#include "field.h"
#include "texture-cache.h"
#include "vect.h"
#include "switch-handler.h"
#include "interface.h"

#include <array>
#include <tuple>



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
    Inactive, //!< Temporarily doesn't interact with anything
    Destroy, //!< Object should be destroyed
    Acting,  //!< Currently interacting with something
    Invulnerable  //!< Unable to interact with anything
  };
  enum SW {  //Correlates switch array positions to names
    Appear = 0, //!< Makes the object appear
    Disappear = 1, //!< Makes the object disappear
    A = 2, //!< Used for activating other objects
    B = 3, //!< 
    In1 = 4, //!< Other objects trigger these
    In2 = 5, //!< These two need better names
    Gen = 6, //!< Currently Unused
    Remove = 7 //!< Object activates this upon collection, death, etc.
  };

  //! Object's motion type
  virtual int Type() {return Object::Intangible;} 
  //! Object's string identifier -- replaces the ObjContainer::Type system
  virtual std::string Name() {return "obj";}
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
  //! get all switches
  std::array<int, 8> getSwitches() const;
  //! get specified arg
  int getArg(std::size_t) const;
  //! get the obj's status
  Status getStatus() const;
  //! Resets the obj's status
  void resetStatus();
  //! get the text
  std::string getText() const;
  //! set all switches at once
  void setSwitches(std::array<int, 8>);
  //! set a specific switch
  void setSwitch(unsigned index, int value);
  //! set the text of the object
  void setText(const std::string&);
  //! set all args at once. Probably unnecessary
  void setArgs(std::array<int, 8>);
  //! set the link id of the object
  void setLinkID(int);
  //! set the parent id of the object
  void setParentID(int);
  //! set the texture id
  void setTextureID(int);
  //! set value of specified arg
  void setArg(std::size_t slot, int v);
  //! empty constructor
  Object();
  //! uid constructor
  Object(int);

protected:
  //can be preset
  int link_id; //!< an id used for interactions between objects
  int texture_id; //!< Used to select a texture for the object
  int parent_id; //!< Should contain the Link ID of the parent. Only used for child objs
  std::array<int, 8> args; //!< Arguments for each object, usage varies by object type
  //yes, I did just borrow SMG's obj_arg system, I couldn't think of a better way
  
  std::array<int, 8> switches;
  std::string text; //!< A text field that can be used by the object.

  //internal, aren't preset

  const int unique_id=-1; //!< Used internally to differentiate objects
  std::array<int, 4> vars; //!< Internal variables, used similarly to args

  Status status; //!< Internal status, can be externally read but not written
};

#endif
