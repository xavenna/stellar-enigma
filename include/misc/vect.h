#ifndef VECT_H
#define VECT_H
#include <cmath>
#include <SFML/System.hpp>
#include "util.h"

#define PI 3.141592653589793238462643383

//! Converts from degrees to radians
float RAD(float);

//! Converts from radians to degrees
float DEG(float);

//! A Two-dimensional vector
template <typename T> class Vector2 {
public:
  T x; //!< x component of vector
  T y; //!< y component of vector
  Vector2<T> operator+(Vector2); //!< Adds two vectors
  Vector2<T> operator+=(Vector2); //!< Adds and assigns two vectors
  Vector2<T> operator-(Vector2); //!< Subtracts two vectors
  Vector2<T> operator-=(Vector2); //!< Subtracts and assigns two vectors 

  T operator+(); //!< Returns the magnitude of the vector
  Vector2<T> operator!(); //!< Returns the normalized vector
  Vector2<T> operator!=(Vector2); //!< Returns the normalized vector
  Vector2<T> operator-(); //!< Returns the negated vector

  Vector2<T> operator*(T); //!< Multiplies by a constant
  Vector2<T> operator*=(T); //!< Multiplies by a constant and assigns
  Vector2<T> operator/(T); //!< Divides by a constant
  Vector2<T> operator/=(T); //!< Divides by a constant and assigns

  T operator|(Vector2); //!< Dot product

  void scale(T); //!< Scales vector to have a given length

  Vector2<T>(T, T);
  Vector2<T>();
private:
  T mag();
};

template <typename T> Vector2<T> Vector2<T>::operator+(Vector2<T> v) {
  return Vector2<T>(x + v.x, y+v.y);
}
template <typename T> Vector2<T> Vector2<T>::operator+=(Vector2<T> v) {
  x += v.x;
  y += v.y;
  return Vector2<T>(x,y);
}
template <typename T> Vector2<T> Vector2<T>::operator-(Vector2<T> v) {
  return Vector2<T>(x - v.x, y - v.y);
}
template <typename T> Vector2<T> Vector2<T>::operator-=(Vector2<T> v) {
  x -= v.x;
  y -= v.y;
  return Vector2<T>(x,y);
}

template <typename T> T Vector2<T>::operator+() {
  return mag();
}
template <typename T> T Vector2<T>::mag() {
  return std::sqrt(x*x + y*y);
}
template <typename T> Vector2<T> Vector2<T>::operator-() {
  return Vector2<T>(-x, -y);
}
template <typename T> Vector2<T> Vector2<T>::operator!() {
  return Vector2<T>(x/mag(), y/mag());
}
template <typename T> Vector2<T> Vector2<T>::operator!=(Vector2<T> v) {
  x /= mag();
  y /= mag();
  return Vector2<T>(x, y);
}

template <typename T> Vector2<T> Vector2<T>::operator*(T n) {
  return Vector2<T>(x*n , y*n);
}
template <typename T> Vector2<T> Vector2<T>::operator*=(T n) {
  x *= n;
  y *= n;
  return Vector2<T>(x,y);
}
template <typename T> Vector2<T> Vector2<T>::operator/(T n) {
  return Vector2<T>(x/n, y/n);
}
template <typename T> Vector2<T> Vector2<T>::operator/=(T n) {
  x /= n;
  y /= n;
  return Vector2<T>(x,y);
}

template <typename T> T Vector2<T>::operator|(Vector2<T> v) {
  return x*v.x + y*v.y;
}

template <typename T> void Vector2<T>::scale(T factor) {
  x /= +(*this);
  y /= +(*this);
}


template <typename T> Vector2<T>::Vector2(T n, T m) {
  x = n;
  y = m;
}

template <typename T> Vector2<T>::Vector2() {
  x = 0;
  y = 0;
}


//! Three-dimensional version of Vector
template <typename T> class Vector3 {
public:
  T x; //!< First component of vector
  T y; //!< Second component of vector
  T z; //!< Third component of vector
  Vector3<T>operator+(Vector3<T>); //!< Adds two vectors
  Vector3<T>operator+=(Vector3<T>); //!< Adds and assigns two vectors
  Vector3<T>operator-(Vector3<T>); //!< Subtracts two vectors
  Vector3<T>operator-=(Vector3<T>); //!< Subtracts and assigns two vectors 

  T operator+(); //!< Returns the magnitude of the vector
  Vector3<T>operator!(); //!< Returns the normalized vector
  Vector3<T>operator!=(Vector3<T>); //!< Returns the normalized vector
  Vector3<T>operator-(); //!< Returns the negated vector

  Vector3<T>operator*(T); //!< Multiplies by a constant
  Vector3<T>operator*=(T); //!< Multiplies by a constant and assigns
  Vector3<T>operator/(T); //!< Divides by a constant
  Vector3<T>operator/=(T); //!< Divides by a constant and assigns

  T operator|(Vector3<T>); //!< Dot product
  Vector3<T>operator^(Vector3<T>); //!< Cross product of two vectors

  void scale(T); //!< Scales vector to have a given length

  Vector3<T>(T, T, T); //!< Value Constructor
  Vector3<T>(); //!< Empty constructor
private:
  T mag(); //!< Magnitude of vector
  Vector3<T> norm(); //!< Normalized vector
};

template <typename T> Vector3<T> Vector3<T>::operator+(Vector3<T> v) {
  return Vector3<T>(x + v.x, y+v.y, z+v.z);
}
template <typename T> Vector3<T>Vector3<T>::operator+=(Vector3<T> v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return Vector3<T>(x,y,z);
}
template <typename T> Vector3<T> Vector3<T>::operator-(Vector3<T> v) {
  return Vector3<T>(x - v.x, y - v.y, z - v.z);
}
template <typename T> Vector3<T>Vector3<T>::operator-=(Vector3<T> v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return Vector3<T>(x,y,z);
}

template <typename T> T Vector3<T>::operator+() {
  return mag();
}
template <typename T> T Vector3<T>::mag() {
  return std::sqrt(x*x + y*y + z*z);
}
template <typename T> Vector3<T> Vector3<T>::operator-() {
  return Vector3<T>(-x, -y, -z);
}
template <typename T> Vector3<T> Vector3<T>::norm() {
  return Vector3<T>(x/mag(), y/mag(), z/mag());
}
template <typename T> Vector3<T> Vector3<T>::operator!() {
  return norm();
}
template <typename T> Vector3<T> Vector3<T>::operator!=(Vector3<T> v) {
  x /= mag();
  y /= mag();
  z /= mag();
  return Vector3<T>(x,y,z);
}

template <typename T> Vector3<T> Vector3<T>::operator*(T n) {
  return Vector3<T>(x*n, y*n, z*n);
}
template <typename T> Vector3<T> Vector3<T>::operator*=(T n) {
  x *= n;
  y *= n;
  z *= n;
  return Vector3<T>(x,y,z);
}
template <typename T> Vector3<T> Vector3<T>::operator/(T n) {
  return Vector3<T>(x/n, y/n, z/n);
}
template <typename T> Vector3<T> Vector3<T>::operator/=(T n) {
  x /= n;
  y /= n;
  z /= n;
  return Vector3<T>(x,y,z);
}

template <typename T> T Vector3<T>::operator|(Vector3<T> v) {
  return x*v.x + y*v.y + z*v.z;
}
template <typename T> Vector3<T> Vector3<T>::operator^(Vector3<T> v) {
  return Vector3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

template <typename T> void Vector3<T>::scale(T factor) {
  *this = factor*norm();
}


template <typename T> Vector3<T>::Vector3(T n, T o, T p) {
  x = n;
  y = o;
  z = p;
}

template <typename T> Vector3<T>::Vector3() {
  x = 0;
  y = 0;
  z = 0;
}



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
  return Vector3<T>(v.y*w.z - v.z*w.y, v.z*w.x - v.x*w.z, v.x*w.y - v.y*w.x);
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

template <typename T> sf::Vector2<T> cMult(sf::Vector2<T> u, sf::Vector2<T> v) {
  return sf::Vector2<T>(u.x * v.x, u.y * v.y);
}

template <typename T> sf::Vector2<T> cDiv(sf::Vector2<T> u, sf::Vector2<T> v) {
  return sf::Vector2<T>(u.x / v.x, u.y / v.y);
}

template <typename T> sf::Vector2<T> vsign(sf::Vector2<T> v) {
  return sf::Vector2<T>{static_cast<float>(sign<T>(v.x)), static_cast<float>(sign<T>(v.y))};
}

// linear algebra stuff

//! Finds the determinant of a 2D matrix, given as columns
template <typename T> T det2(sf::Vector2<T> c1, sf::Vector2<T> c2) {
  return c1.x * c2.y - c1.y * c2.x;
}

//! Solves a 2D matrix given as columns. Excepts if no unique solution exists
sf::Vector2f systemsolve2(sf::Vector2f c1, sf::Vector2f c2, sf::Vector2f c);


#endif
