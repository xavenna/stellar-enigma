#include "util.h"

// util: various utility functions used by other parts of the code
bool isNum(const std::string& st) {
  for( auto x : st) {
    if(!isdigit(x))
      return false;
  }
  return true;
}
std::string lowercase(const std::string& in) {
  std::string e;
  for(auto x : in) {
    e += std::tolower(x);
  }
  return e;
}
