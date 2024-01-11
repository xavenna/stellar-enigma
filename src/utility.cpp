#include "utility.h"



int Utility::getRandInt(int m, int n) {
  std::uniform_int_distribution<int> dist(m, n);
  return dist(engine);
}
float Utility::getRandFloat(float m, float n) {
  std::uniform_real_distribution<float> dist(m, n);
  return dist(engine);
}

Utility::Utility() : engine{rd()} {
}
