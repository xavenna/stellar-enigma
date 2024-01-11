#ifndef UTILITY_H
#define UTILITY_H
#include <random>

//! A class that provides miscellaneous utilities, such as centralized rng
class Utility {
public:
  int getRandInt(int m, int n); //!< Returns an int in the range [m,n)
  float getRandFloat(float, float); //!< Returns a float in the range [m,n)
  Utility();
private:
  //! random_device is broken on Windows MinGW - gcc 9. I think the bug is limited to that version, though
  std::random_device rd;
  std::mt19937 engine;
};

#endif
