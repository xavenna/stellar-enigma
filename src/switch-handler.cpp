#include "switch-handler.h"

void SwitchHandler::write(const unsigned index, const bool state) {
  if(index > 255) { //index is unsigned, so no undervalue check is required
    std::cerr << "Error: Out of bounds switch write\n";
    return;  //do nothing in case of an illegal write
  }
  binSw.set(index, state);
}
bool SwitchHandler::read(const unsigned index) const {
  if(index > 255) {
    std::cerr << "Error: Out of bounds switch read\n";
    return false;
  }
  return binSw.test(index);
}
