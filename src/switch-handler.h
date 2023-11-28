#ifndef SWITCH_HANDLER_H
#define SWITCH_HANDLER_H
#include <bitset>
#include <iostream>
#include <vector>
#include <array>


class SwitchHandler {
  //the switches themselves
public:
  void write(const unsigned, const bool);
  bool read(const unsigned) const;
private:
  std::bitset<256> binSw; //this should be enough for now
};

#endif
