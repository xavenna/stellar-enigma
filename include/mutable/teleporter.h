#ifndef TELEPORTER_H
#define TELEPORTER_H
#include "mutable/object.h"
#include "mutable/player.h"

//! An object that teleports objects to a specific destination.
/*!
 *  Arguments:
 *  uint args[0] : activation time.\n
 *    How many frames of contact are required for teleportation to occur.
 *
 *  uint args[1] : mode:\n
 *    0 - only teleports player\n
 *    1 - teleports player & Entities\n
 *    2 - teleports all objects\n
 *  uint (bits) args[2] : behavior:\n
 *    0x1 - bool: true if despawns after one teleport\n
 *    0x2 - bool: true if teleport only works if SW_A is active\n
 *
 *  uint args[3] : display\n
 *    0 - invisible\n
 *    1 - displays a light grey rectangle\n
 *    2 - displays a texture??? Possible future feature\n
 *
 *  uint args[4] : cooldown time: How long after a teleportation does it take to recharge
 *
 *  int args[6] : destination xpos //this should use a different system eventually
 *
 *  int args[7] : destination ypos
 *
 *  Switches:\n
 *  SW_A : controls activation state\n
 *  SW_B : activates when teleport is triggered
 */
class Teleporter : public Object {
public:
  virtual Interface interact(Object*, Field*, SwitchHandler*);
  virtual CacheNodeAttributes draw();
  virtual Interface behave(SwitchHandler*, Utility*);
  virtual bool verify();
  
  virtual std::string Name() const {return "teleporter";}
  virtual sf::Vector2f BaseSize() const {return sf::Vector2f(32,32);}
  virtual Object::MotionType Type() const {return Object::Intangible;}
  virtual bool grabbable() const {return false;}
  virtual int priority() const {return 32;}
  Teleporter(int);

protected:
  int cooldown; //!< If positive, don't activate
  unsigned chargeup; //!< Counts up time until activation

};

#endif
