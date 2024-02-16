#ifndef SAVE_H
#define SAVE_H

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <array>

#include <json11.hpp>

#include "util.h"

//! Each object's unique variable pool.
typedef std::array<int, 16> Vars;
class Object;
class Player;

//! A place for global variables to be stored.
/* Each object (and player) gets a slot. Only the slot's owner can write to a slot,
* but all objects can read from it.
*
* It exists to allow objects to broadcast status to all others.
*/
class VarPool {
public:
  //! Writes to an object's slot. The reference is to prevent illegal wrong-writes.
  void write(const Object*, unsigned index, int data);
  //! Writes to player's slot
  void write(const Player*, unsigned index, int data);

  //! Reads a variable from specified object's slot
  int read(unsigned id, unsigned index) const;

  //! Reads one of the player's variables
  int playerRead(unsigned index) const;

private:
  std::map<unsigned, Vars> registry;
  Vars playerVars;

};

//! The Savedata controller
/*!
 *  The savedata is stored as JSON, in save/save[slotid].sesave
 *  slotid is filled with the slotName variable
 */
class SaveController {
public:
  //! Serializes and writes data to save file.
  int writeData();
  //! reads and deserializes data from save file.
  int readData();

  //! Check if string variable exists
  bool hasStr(const std::string&) const noexcept;
  //! Check if bool variable exists
  bool hasBool(const std::string&) const noexcept;
  //! Check if float variable exists
  bool hasFloat(const std::string&) const noexcept;
  //! Check if integer variable exists
  bool hasInt(const std::string&) const noexcept;

  //! get string variable
  std::string getStr(const std::string&, bool&) const noexcept;
  //! get int variable
  int getInt(const std::string&, bool&) const noexcept;
  //! get float variable
  float getFloat(const std::string&, bool&) const noexcept;
  //! get bool variable
  bool getBool(const std::string&, bool&) const noexcept;

  //! get string variable
  std::string getStr(const std::string&) const;
  //! get int variable
  int getInt(const std::string&) const;
  //! get float variable
  float getFloat(const std::string&) const;
  //! get bool variable
  bool getBool(const std::string&) const;

  //! set string variable
  void setStr(const std::string&, const std::string&);
  //! set int variable
  void setInt(const std::string&, int);
  //! set float variable
  void setFloat(const std::string&, float);
  //! set bool variable
  void setBool(const std::string&, bool);

  void setSlot(const std::string&);
protected:
  //! Internal function to get the filename of selected slot
  std::string filename();

  //! Maximum numbers of int variables that can be stored
  const unsigned iMax=65536;
  //! Maximum numbers of float variables that can be stored
  const unsigned fMax=65536;
  //! Maximum numbers of bool variables that can be stored
  const unsigned bMax=262144;
  //! Maximum numbers of string variables that can be stored
  const unsigned sMax=512;

  //! Maximum length of each string variable
  const unsigned slMax=128;

  //! String specifying which save slot to use - appended to filename
  std::string saveSlot; 
  // The actual save data:
  std::map<std::string, int> ivars; //!< Saved int variables
  std::map<std::string, float> fvars; //!< Saved float variables
  std::map<std::string, bool> bvars; //!< Saved bool variables
  std::map<std::string, std::string> svars; //!< Saved string variables

  VarPool vars;  
};
#endif
