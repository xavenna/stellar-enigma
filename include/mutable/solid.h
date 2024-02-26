#ifndef SOLID_H
#define SOLID_H
#include "mutable/object.h"


//! An object that is solid and blocks movement
class Solid : public Object {
public:
  virtual Interface interact(Player*, Field*, SwitchHandler*);
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw(const TextureCache*);
  virtual std::string Name() {return "solid";}
  virtual int priority() {return 0;}
  virtual int Type() {return Object::Static;} 
  virtual bool verify();
  Solid(int);
protected:
  //! A utility variable used during interaction.
  /*!
   *  Basically, the point of this is to enable classes derived from Solid to use
   *  Solid::interact, rather than copy-pasting the code. This cleans up the code by
   *  only having this piece of code defined in one place.\n 
   *  When a derived object's interact function is called, it calls Solid::interact.
   *  When it returns, the derived object checks the value of interacting. If it's true,
   *  an interaction happened. Otherwise, no interaction occurred.
   *  
   *  Anyways, this functionality might be removed soon, as the old way of determining
   *  interactions leads to the hollow-interior bug, and Solid::interact needs to be 
   *  rewritten to fix it. Anyways...
   */
  bool interacting=false;
};


#endif
