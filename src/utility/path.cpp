#include "utility/path.h"

PathSeg::PathSeg(sf::Vector2f s, sf::Vector2f e, sf::Vector2f co1, sf::Vector2f co2) {
  a = s;
  b = e;
  c = co1;
  d = co2;
}


sf::Vector2f PathSeg::inter(float t) const {
  return a + t * (b-a);
  //return a + t * (3.f*b - 3.f*a) + t*t*(3.f*a -6.f*b + 3.f*c) + t*t*t*(-a + 3.f*b - 3.f*c + d);
  // don't smoothstep this, you absolute buffoon!
}


sf::Vector2f Path::inter(float t) const {
  float m = segments.size() * t;
  auto s = segments[static_cast<int>(m)];
  return s.inter(m - std::floor(m));

}


