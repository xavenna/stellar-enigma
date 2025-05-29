#include "utility/utility.h"


unsigned Utility::getFrame() {
  return counter;
}
void Utility::nextFrame() {
  counter++;
}

int Utility::getRandInt(int m, int n) {
  std::uniform_int_distribution<int> dist(m, n);
  return dist(engine);
}
float Utility::getRandFloat(float m, float n) {
  std::uniform_real_distribution<float> dist(m, n);
  return dist(engine);
}

#ifndef RAND_USE_TIME
Utility::Utility() : engine{rd()} {
}
#else
Utility::Utility() : engine(std::chrono::system_clock::now().time_since_epoch().count()) {
}
#endif
