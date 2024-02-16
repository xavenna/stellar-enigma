#ifndef UTILITY_H
#define UTILITY_H
#include <random>
#include "save.h"

//! A class that provides miscellaneous utilities, such as centralized rng
class Utility {
public:
  int getRandInt(int m, int n); //!< Returns an int in the range [m,n)
  float getRandFloat(float, float); //!< Returns a float in the range [m,n)
  SaveController save;
  Utility();
private:
  //! random_device is broken on Windows MinGW - gcc 9. I think the bug is limited to that version, though
  /*!
   *  A workaround needs to be setup for windows
   */
  std::random_device rd;
  std::mt19937 engine;
};

#endif
