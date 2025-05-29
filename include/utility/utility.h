#ifndef UTILITY_H
#define UTILITY_H
#include <random>
#include <chrono>
#include "utility/save.h"

//! A class that provides miscellaneous utilities, such as centralized rng
class Utility {
public:
  int getRandInt(int m, int n); //!< Returns an int in the range [m,n)
  float getRandFloat(float, float); //!< Returns a float in the range [m,n)
  unsigned getFrame();
  void nextFrame();
  SaveController save;
  VarPool varpool;
  Utility();
private:
  //! random_device is broken on Windows MinGW - gcc 9. I think the bug is limited to that version, though
  /*!
   * If compiling on windows, set the RAND_TIME variable in the makefile to 1. This uses
   * the system time to seed the generator.
   */
  std::random_device rd;
  std::mt19937 engine;
  unsigned counter;
};

#endif
