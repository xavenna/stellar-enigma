#include "util.h"
bool isNum(std::string st) {
  for( auto x : st) {
    if(!isdigit(x))
      return false;
  }
  return true;
}
std::string lowercase(std::string in) {
  std::string e;
  for(auto x : in) {
    e += std::tolower(x);
  }
  return e;
}
