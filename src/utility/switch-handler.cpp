#include "utility/switch-handler.h"

void SwitchHandler::write(const int index, const bool state) {
  if(index > 255 || index < 0) { //index is unsigned, so no undervalue check is required
    std::cerr << "Error: Out of bounds switch write\n";
    return;  //do nothing in case of an illegal write
  }
  binSw.set(static_cast<unsigned>(index), state);
}
bool SwitchHandler::read(const int index) const {
  if(index > 255 || index < 0) {
    std::cerr << "Error: Out of bounds switch read\n";
    return false;
  }
  return binSw.test(static_cast<unsigned>(index));
}
