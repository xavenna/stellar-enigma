#ifndef UTIL_H
#define UTIL_H
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cmath>

//! Takes a string and turns it into lowercase
std::string lowercase(const std::string&);
//! determines whether n is contained in key
bool ifMatch(char n, std::string key);
//! returns true if supplied string is a number
bool isNum(const std::string& st);
//! returns true if specified string is a boolean ("0" or "1")
bool isBool(const std::string& st);
//! returns true if specified char is whitespace
bool isEmpty(char c);
//! breaks n into a vector, starting a new element wherever a component of key is found
void parse(const std::string& n, std::vector<std::string>& th, std::string key = "./\\-");


bool isInt(float s);
bool isInt(double s);
//this function was taken from stackoverflow
template <typename T> int sign(T val) {
  return (T(0) < val) - (val < T(0));
}

template <typename T> T dist(T u, T v) {
  return std::sqrt(std::pow(u, 2) + std::pow(v, 2));
}

template <typename T> std::vector<T> vecCat(const std::vector<T>& i, const std::vector<T>& j) {
  std::vector<T> res;
  res.reserve(i.size() + j.size());

  res = i;
  res.insert(i.end(), j.begin(), j.end());
  return res;
}

bool getEntireFile(std::ifstream& in, std::string& out);
#endif
