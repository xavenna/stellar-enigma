#ifndef UTIL_H
#define UTIL_H
#include <ctime>
#include <string>
#include <vector>

//Documnted

//! Takes a string and turns it into lowercase
std::string lowercase(const std::string&);
//! determines whether n is contained in key
bool ifMatch(char n, std::string key);
//! returns true if supplied string is a number
bool isNum(const std::string& st);
bool isBool(const std::string& st);
//! breaks n into a vector, breaking wherever a component of key is found
void parse(const std::string& n, std::vector<std::string>& th, std::string key = "./\\-");
#endif
