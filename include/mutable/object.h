#ifndef OBJECT_H
#define OBJECT_H

#include "mutable/mutable.h"
#include "level/field.h"
#include "utility/texture-cache.h"
#include "misc/vect.h"
#include "utility/switch-handler.h"
#include "misc/interface.h"
#include "utility/utility.h"
#include "utility/save.h"
#include "misc/util.h"

#include <array>

class Player;

//! A class for objects, anything with dynamic behavior
/*!
 *  Extends mutable with additional attributes specific to objects.
 *  To create an object, extend this class, redefining any necessary virtual functions
 * 
 *  Virtual functions:\n
 *  behave - called every frame, normal behavior occurs here\n
 *  interact - called when something interacts with the object\n
 *  draw - tells the engine how to draw the object
 *  
 *  In the future, solid objects will have a list of walls, used in addition to the map
 *  wall list in the interaction handler
 *
 */
class Object : public Mutable {
public:
  //! Constant for infinite mass
  //static const int infinite=-1;
  //! Constant for intangible object's mass
  //static const int intangible=-2;
  //! Object's motion type
  enum MotionType {
    Static, //!< Objects that can't be moved
    Sliding, //!< Can be moved by others
    Intangible, //!< Doesn't interact with other objects
    Entity, //!< Something that moves by itself
    Play //!< Reserved for the player
  };
  //! Object's hitbox shape.
  enum Shape {
    Rect, //!< Standard rectangular.
    Circle //!< Circular - pos represents center, size.x is radius, size.y is unused
  };
  enum Status {
    Normal, //!< No special conditions
    Inactive, //!< Temporarily doesn't interact with anything
    Destroy, //!< Object should be destroyed
    Acting,  //!< Currently interacting with something
    Acted,   //!< Being interacted with
    Grabbing,  //!< Grabbing
    Held,      //!< Being held
    Invulnerable,  //!< Unable to interact with anything
    Moving,  //!< The object is moving by itself
    Pushed,  //!< The object is being pushed by something else
    Squished,  //!< The object is being squished
    Damaged,  //!< The object is being damaged by another object
    PushBack   //!< The object is being pushed back by a solid object
  };
  enum SW {  //Correlates switch array positions to names
    Appear = 0, //!< Makes the object appear
    Disappear = 1, //!< Makes the object disappear
    A = 2, //!< Generic Switch
    B = 3, //!< Generic Switch
    C = 4, //!< Generic Switch
    D = 5, //!< Generic Switch
    Stat = 6, //!< Causes object to enter a different behavior mode (eg. slow time)
    Remove = 7 //!< Object activates this upon collection, death, etc.
  };

  //object info functions

  //! Object's motion type
  virtual Object::MotionType Type() const {return Object::Intangible;} 
  //! Object's shape
  virtual int Shape() const {return Object::Rect;}
  //! Object's string identifier -- replaces the ObjContainer::Type system
  virtual std::string Name() const {return "obj";}
  //! Object's base size in pixels
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(0,0);};
  //! Vector pointing from hitbox origin to sprite origin
  virtual sf::Vector2f Offset() const {return sf::Vector2f(0,0);};
  //! object interaction priority
  virtual int priority() const {return 32;} //0 is highest, decreases counting up
  //! Can the object be grabbed (SM64 style)?
  virtual bool grabbable() const {return false;}
  //! Can the object be used by player
  virtual bool usable() const {return false;}

  //object behavior functions

  //! base version of interact();
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  //! produces the proper CacheNodeAttributes for the state of the object
  virtual CacheNodeAttributes draw();
  //! base version of behave();
  virtual Interface behave(SwitchHandler*, Utility*);
  //! base version of verify, which ensures object config is valid
  virtual bool verify();
  //! used for the objcontainer to send complex messages to parents (maybe more later)
  virtual void notify(msg);

  //! base version of grab. Returns true if object is grabbed by player
  virtual bool use(Player*);

  //other functions

  //! Gets effective size of object (basesize * scale)
  sf::Vector2f getESize() const;
  //! get the center of the mutable
  sf::Vector2f getCenter() const;
  //! get the hitbox rectangle
  sf::FloatRect getBounds() const;
  //! get the sprite outline rectangle
  sf::FloatRect getSpriteBounds() const;

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
  //! Sets the obj's status.
  /*!
   *  Eventually, I'd like to do away with this, but for now it seems necessary.\n
   *  As the interaction handler needs to update the object's state, either external
   *  access is needed, or I have to add yet another virtual for updating state that is
   *  run each iteration of the handler. For now, this is a passable solution. Maybe it
   *  should be a friend function or something?
   */
  void setStatus(Object::Status);
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

  void setPushback(Direction d);
  void resetPushback();

  bool readPushback(Direction d);

  bool held=false;
protected:
  //can be preset
  int link_id; //!< an id used for interactions between objects
  int texture_id; //!< Used to select a texture for the object
  int parent_id; //!< Should contain the Link ID of the parent. Only used for child objs
  std::array<int, 8> args; //!< Arguments for each object, usage varies by object type
  //yes, I did just borrow SMG's obj_arg system, I couldn't think of a better way
  
  std::array<int, 8> switches;
  std::string text; //!< A text field that can be used by the object.

  sf::Vector2f pushbackVect; //!< Used for motion handling
  DirectionalBool pushback; //!< Pushback status
  //internal, aren't preset

  const int unique_id=-1; //!< Used internally to differentiate objects
  //! Internal variables, used similarly to args. Deprecated.
  /*! Going forward, objects can define additional member variables. Since objects are
   * stored in the heap, they don't need to all have the same size.
   */
  std::array<int, 4> vars;

  Status status; //!< Internal status, can be externally read but not written
};

#endif
