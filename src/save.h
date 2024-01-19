#ifndef SAVE_H
#define SAVE_H

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <fstream>

#include <json11.hpp>

#include "util.h"


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
  std::string filename();

  //! String specifying which save slot to use - appended to filename
  std::string saveSlot; 
  //! The actual save data
  std::map<std::string, int> ivars;
  std::map<std::string, float> fvars;
  std::map<std::string, bool> bvars;
  std::map<std::string, std::string> svars;
};
#endif
