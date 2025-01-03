#ifndef OBJ_CONTAINER_H
#define OBJ_CONTAINER_H
#include <list>
#include "mutable/object.h"

//! A class for holding the object list, as well as serving as an Object Factory
/*!
 *  Holds a list of pointers to objects. The pointers are dynamically allocated, and
 *  freed by the destructor. Objects can be added as empty, by copy, or by traits
 *  Objects can be returned by value, by pointer, or by reference, depending on situation
 */
class ObjContainer {
public:
  //! Get reference to specified object
  Object& getObjRef(unsigned);
  //! Get pointer to specified object
  Object* getObjPtr(unsigned);
  //! Get copy of specified object
  Object getObj(unsigned) const;
  //! Get pointer to obj by link id
  Object* getObjByID(int);

  //! Specifies if object with specified link id is contained in container
  bool hasObj(int);
  bool hasUID(int);
  bool hasObjPtr(const Object*);

  //! Create copy of passed object, as specified type
  /*!
   *  This function is dynamically generated by the prebuild command, based on the
   *  headers in include/mutable/
   */
  bool storeObj(Object ob, std::string, std::string& status);
  
  //! Create empty Object of specified type. Wrapper to storeObj(Object, string)
  bool storeObj(std::string);

  //! Remove specified object and free its allocated memory
  void removeObj(unsigned);
  //! Remove object located at specified pointer, if object is stored in container
  void removeObj(Object*);
  //! Send notification to object.
  bool notify(msg);

  //! Clears objects to prepare for level load
  void clearObjects();

  //! Get the number of objects stored
  std::size_t size() const;
  //! Constructs object container
  ObjContainer();
  //! Destructor; Frees all object slots
  ~ObjContainer();
private:
  //! where pointers are stored internally; ObjContainer owns the objects
  std::list<Object*> list;
  //! Increments whenever an object is created, used to create unique identifiers
  long unsigned counter;
};

bool is_valid_obj_name(const std::string&);

#endif
