#ifndef VECT_H
#define VECT_H
#include <cmath>

template <typename T> class Vector2 {
public:
  T x;
  T y;
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


template <typename T> class Vector3 {
public:
  T x;
  T y;
  T z;
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

  Vector3<T>(T, T, T);
  Vector3<T>();
private:
  T mag();
  Vector3<T> norm();
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

#endif 
