#ifndef VECT_H
#define VECT_H
#include <cmath>
#include <array>
#include <SFML/System.hpp>
#include "util.h"

// NOTE: SFML v3 added functions for most of the stuff here, so it'll be deprecated

#define PI 3.141592653589793238462643383

//! Converts from degrees to radians
float RAD(float);

//! Converts from radians to degrees
float DEG(float);





//add some additional functions to sf::Vector2,3?

//! Gets the magnitude of a sf::Vector2
template <typename T> float magnitude(sf::Vector2<T> v) {
  return std::sqrt(v.x * v.x + v.y * v.y);
}
//! Calculates the magnitude of a sf::Vector3
template <typename T> float magnitude(sf::Vector3<T> v) {
  return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

//! returns the input vector, normalized
template <typename T> sf::Vector2<T> norm(sf::Vector2<T> v) {
  if(magnitude(v) == 0) {
    return v;
  }
  return v / magnitude(v);
}

//! Calculates the dot product of the two 2D vectors
template <typename T> T dp(sf::Vector2<T> v, sf::Vector2<T> w) {
  return v.x*w.x + v.y*w.y;
}

//! Calculates the dot product of the two 3D vectors
template <typename T> T dp(sf::Vector3<T> v, sf::Vector3<T> w) {
  return v.x*w.x + v.y*w.y + v.z*w.z;
}

//! Calculates the cross product of the two 3D vectors
template <typename T> sf::Vector3<T> xp(sf::Vector3<T> v, sf::Vector3<T> w) {
  return sf::Vector3<T>(v.y*w.z - v.z*w.y, v.z*w.x - v.x*w.z, v.x*w.y - v.y*w.x);
}

//! Calculates the magnitude of the cross product of the two vectors
template <typename T> T xpm(sf::Vector2<T> v, sf::Vector2<T> w) {
  return std::abs(v.x*w.y - v.y*w.x);
}
template <typename T> T xp(sf::Vector2<T> v, sf::Vector2<T> w) {
  return (v.x*w.y - v.y*w.x);
}

//! gets the angle of specified vector
template <typename T> float angle(sf::Vector2<T> v) {
  return angle(v, sf::Vector2<T>(1,0));
}
//! Gets the angle between two vectors, in degrees
template <typename T> float angle(sf::Vector2<T> v, sf::Vector2<T> w) {
  if(magnitude(v) == 0 || magnitude(w) == 0) {
    return 0;
  }
  return DEG( std::acos(dp(v, w) / (magnitude(v)*magnitude(w)) ) );
}
template <typename T> float angle(sf::Vector3<T> v, sf::Vector3<T> w) {
  if(magnitude(v) == 0 || magnitude(w) == 0) {
    return 0;
  }
  return DEG (std::acos( dp(v,w) / (magnitude(v)*magnitude(w)) ) );
}

//! projects source onto target
template <typename T> sf::Vector2<T> proj(sf::Vector2<T> source, sf::Vector2<T> target) {
  if(magnitude(source) == 0) {
    return sf::Vector2<T>(0,0);
  }
  T mag = dp(source, target) / (magnitude(target));
  return norm(target) * mag;
}

//! projects source onto target
template <typename T> sf::Vector3<T> proj(sf::Vector3<T> source, sf::Vector3<T> target) {
  if(magnitude(source) == 0) {
    return sf::Vector3<T>(0,0,0);
  }
  T magnitude = dp(source, target) / (magnitude(target));
  return norm(target) * magnitude;
}

template <typename T> sf::Vector2<T> normal(sf::Vector2<T> v) {
  return sf::Vector2<T>(-v.y, v.x);
}

//! Calculates the abs of each element separatelg
template <typename T> sf::Vector2<T> vabs(sf::Vector2<T> v) {
  return sf::Vector2<T>(std::abs(v.x), std::abs(v.y));
}
template <typename T> sf::Vector2<T> zero2() {
  return sf::Vector2<T>{0,0};
}
template <typename T> sf::Vector3<T> zero3() {
  return sf::Vector3<T>{0,0,0};
}

//! Component-wise multiplication.
template <typename T> sf::Vector2<T> cMult(sf::Vector2<T> u, sf::Vector2<T> v) {
  return sf::Vector2<T>(u.x * v.x, u.y * v.y);
}

//! Component-wise division
template <typename T> sf::Vector2<T> cDiv(sf::Vector2<T> u, sf::Vector2<T> v) {
  return sf::Vector2<T>(u.x / v.x, u.y / v.y);
}

//! Returns a vector of the sign of each component
template <typename T> sf::Vector2<T> vsign(sf::Vector2<T> v) {
  return sf::Vector2<T>{static_cast<float>(sign<T>(v.x)), static_cast<float>(sign<T>(v.y))};
}

template <typename T> sf::Vector2<T> rotateAbout(sf::Vector2<T> p, sf::Vector2<T> origin, sf::Angle ang) {

  sf::Vector2f a = p - origin;
  sf::Vector2f rotPos = {a.x * std::cos(ang.asRadians()) - a.y * std::sin(ang.asRadians()),
        a.x * std::sin(ang.asRadians()) + a.y * std::cos(ang.asRadians())};
  return rotPos + origin;
}

template <typename T> sf::Vector2<T> scaleAbout(sf::Vector2<T> p, sf::Vector2<T> origin,
    float scale) {
  return (scale * (p - origin)) + origin;
}

template <typename T> sf::Vector2<T> transformAbout(sf::Vector2<T> p, sf::Vector2<T>
    origin, sf::Angle ang, float scale) {

  sf::Vector2<T> adjPos = p - origin;
  sf::Vector2f rotPos = {adjPos.x * std::cos(ang.asRadians()) - adjPos.y * std::sin(ang.asRadians()),
    adjPos.x * std::sin(ang.asRadians()) + adjPos.y * std::cos(ang.asRadians())};


  return (scale * rotPos) + origin;

}

// linear algebra stuff

//! Finds the determinant of a 2D matrix, given as columns
template <typename T> T det2(sf::Vector2<T> c1, sf::Vector2<T> c2) {
  return c1.x * c2.y - c1.y * c2.x;
}


//! Solves a 2D matrix given as columns. Excepts if no unique solution exists
sf::Vector2f systemsolve2(sf::Vector2f c1, sf::Vector2f c2, sf::Vector2f c);


#endif
