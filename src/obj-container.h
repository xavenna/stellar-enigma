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
    spawner //!< Spawner, spawns objects on a schedule
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
  //! Remove object located at specified pointer, if object is stored in container
  void removeObj(Object*);

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
