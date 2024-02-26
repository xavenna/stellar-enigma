#ifndef SWITCH_HANDLER_H
#define SWITCH_HANDLER_H
#include <bitset>
#include <iostream>
#include <vector>
#include <array>


//! Manages the Switch system for inter-object communication
/*!
 *  To write to a switch, call write(switch number, value);\n 
 *  Within an object, use this method:\n 
 *  write(switches[SW::Name], value\n 
 *  This method allows an object to call a switch by internal switch variable
 */
class SwitchHandler {
  //the switches themselves
public:
  //! Writes to specified switch
  void write(const int, const bool);
  //! Reads from specified switch
  bool read(const int) const;
private:
  std::bitset<256> binSw; //! The internal representation of the switches
};

#endif
