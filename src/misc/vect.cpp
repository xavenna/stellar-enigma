#include "misc/vect.h"


float RAD(float t) {
  return t * PI / 180;
}

float DEG(float t) {
  return t * 180 / PI;
}


sf::Vector2f systemsolve2(sf::Vector2f c1, sf::Vector2f c2, sf::Vector2f c) {
  if(det2(c1, c2) == 0) {
    throw std::invalid_argument("Error: system has no unique solution");
  }
  return sf::Vector2f(
      det2(c, c2) / det2(c1, c2),
      det2(c1, c) / det2(c1, c2)
  );


}
