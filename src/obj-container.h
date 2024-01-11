#ifndef OBJ_CONTAINER_H
#define OBJ_CONTAINER_H
#include "object.h"
#include "solid.h"
#include "pushable.h"
#include "key.h"
#include "board.h"
#include "spike.h"
#include "toggle-block.h"
#include "cutplay.h"
#include "door.h"

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
    toggle_block, //!<two-state block, toggles upon player contact
    door  //!< Block that toggles tangibility upon switch activation
  };
  //! Get reference to specified object
  Object& getObjRef(unsigned);
  //! Get pointer to specified object
  Object* getObjPtr(unsigned);
  //! Get copy of specified object
  Object getObj(unsigned) const;
  //! Get pointer to obj by link id
  Object* getObjByID(int);
  //! Create copy of passed object, as specified type
  bool storeObj(Object ob, std::string);
  
  //! Create empty Object of specified type. Wrapper to storeObj(Object, string)
  bool storeObj(std::string);

  //! Remove specified object and free its allocated memory
  void removeObj(unsigned);
  //! Remove object located at specified pointer, if object is stored in container
  void removeObj(Object*);
  //! Send notification to object.
  bool notify(msg);

  //! Get the number of objects stored
  std::size_t size() const;
  //! Constructs object container
  ObjContainer();
  //! Destructor; Frees all object slots
  ~ObjContainer();
private:
  //! where pointers are stored internally; ObjContainer owns the objects
  std::deque<Object*> list;
  //! Increments whenever an object is created, used to create unique identifiers
  long unsigned counter;
};

//! Utility function to transform an id to a type
ObjContainer::Type getType(unsigned p);

#endif
