#include "misc/util.h"

// util: various utility functions used by other parts of the code. None of these are stellar-enigma specific


bool isNum(const std::string& st) {
  for(unsigned i=0;i<st.size();i++) {
    if(!isdigit(st[i]) && (st[i] != '-' || i != 0))
      return false;
  }
  return true;
}
bool isBool(const std::string& st) {
  return (st == "0" || st == "1");
}
bool isEmpty(char c) {
  return (c == ' ' || c == '\t'); 
}
std::string lowercase(const std::string& in) {
  std::string e;
  for(auto x : in) {
    e += std::tolower(x);
  }
  return e;
}

bool getEntireFile(std::ifstream& in, std::string& out) {
	if(!in.is_open())
		return false;
    std::ostringstream sstr;
    sstr << in.rdbuf();
	out = sstr.str();
	return true;
}


//returns whether n is found in key
bool ifMatch(char n, std::string key) {
	return (key.find(n) != std::string::npos);
}

//splits a string into a vector of strings
void parse(const std::string& n, std::vector<std::string>& th, std::string key) {
  //each entry is a string delimited by a character in key, similar to bk but more general
  std::string cw;
  for(size_t i=0;i<n.size();i++) {
    if(ifMatch(n[i], key)) {
      if(cw.empty())
	cw += '\0';
      th.push_back(cw);
      cw.clear();
    }
    else {
      cw += n[i];
    }
  }
  th.push_back(cw);
}
