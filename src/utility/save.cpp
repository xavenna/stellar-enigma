#include "utility/save.h"
#include "mutable/object.h"
#include <cmath>


void VarPool::write(const Object* o, unsigned index, int data) {
  if(o == nullptr || o->getLinkID() < 0 || index > 15) {
    std::cerr << "VarPool invalid write\n";
    return;
  }
  registry[static_cast<unsigned>(o->getLinkID())][index] = data;
}

void VarPool::write(const Player* p, unsigned index, int data) {
  if(p == nullptr || index > 15) {
    std::cerr << "VarPool invalid write\n";
    return;
  }
  playerVars[index] = data;
  
}
int VarPool::read(unsigned id, unsigned index) const {
  //if present, 
  if(index > 16 || registry.find(id) == registry.end()) {
    std::cerr << "VarPool invalid read\n";
    return 0;
  }
  // uses at instead of [] because the compiler doesn't like []
  return registry.at(id).at(index);
}


int VarPool::playerRead(unsigned index) const {
  if(index > 16) {
    std::cerr << "VarPool invalid read\n";
    return 0;
  }
  return playerVars[index];
}


int SaveController::writeData() {
  std::ofstream write(filename());
  //convert save data to json
  std::string data = "{\n";
  unsigned ilen = ivars.size();
  unsigned flen = fvars.size();
  unsigned slen = svars.size();
  unsigned blen = bvars.size();
  unsigned i=0; //counter
  for(auto x : ivars) {
    data += "\"" + x.first + "\" : " + std::to_string(x.second);
    if(i == ilen-1 && flen+slen+blen == 0) {
      //this is the last entry, no comma
    }
    else {
      data += ',';
    }
    data += '\n';
    i++;
  }
  i=0;
  for(auto x : bvars) {
    data += "\"" + x.first + "\" : " + (x.second ? "true" : "false");
    if(i == blen-1 && flen+slen == 0) {
      //this is the last entry, no comma
    }
    else {
      data += ',';
    }
    data += '\n';
    i++;
  }
  i=0;
  for(auto x : fvars) {
    data += "\"" + x.first + "\" : " + std::to_string(x.second);
    if(i == flen-1 && slen == 0) {
      //this is the last entry, no comma
    }
    else {
      data += ',';
    }
    data += '\n';
    i++;
  }
  i=0;
  for(auto x : svars) {
    data += "\"" + x.first + "\" : \"" + x.second + "\"";
    if(i == slen-1) {
      //this is the last entry, no comma
    }
    else {
      data += ',';
    }
    data += '\n';
    i++;
  }
  data += "}\n";
  //std::cout << data;
  //serialize produced json
  write << data;
  write.close();
  return -1;
}
int SaveController::readData() {
  //open file data
  std::ifstream read(filename());
  std::string data;
  std::string err;
  if(!read.is_open()) {
    std::clog << "Error: Could not open save file\n";
    return -1;
  }
  getEntireFile(read, data);
  read.close();
  //read contents of save file
  Json::Value k;

  Json::Reader reader;
  reader.parse(data, k);

  //trawl through save, find each thing
  int i;
  float f;
  auto names = k.getMemberNames();
  for(auto x : names) {
    const Json::Value v = k[x];
    if(v.isNumeric()) {
      i = v.asInt();
      f = v.asFloat();
      if(i == int(f)) {
        ivars.emplace(x, v.asInt());
      }
      else {
        fvars.emplace(x, v.asFloat());
      }
    }
    else if(v.isString()) {
      svars.emplace(x, v.asString());
    }
    else if(v.isBool()) {
      bvars.emplace(x, v.asBool());
    }
    else {
      std::clog << "Error: Arrays and nested objects are not currently legal\n";
      return -2;
      break;
    }
  }
  return -1;
}

std::string SaveController::filename() {
  std::string base = "save/save";
  return base + saveSlot + ".sesave";
}

bool SaveController::hasStr(const std::string& name) const noexcept {
  return svars.find(name) != svars.end();
}
bool SaveController::hasBool(const std::string& name) const noexcept {
  return bvars.find(name) != bvars.end();
}
bool SaveController::hasFloat(const std::string& name) const noexcept {
  return fvars.find(name) != fvars.end();
}
bool SaveController::hasInt(const std::string& name) const noexcept {
  return ivars.find(name) != ivars.end();
}


//non-excepting versions
int SaveController::getInt(const std::string& name, bool& s) const noexcept{
  //check if specified variable is stored.
  auto it = ivars.find(name);
  if(it == ivars.end()) {
    //not found in the map
    std::cout << "Error: Int variable '" << name << "' not found\n";
    s = false;
    return -1;
  }
  else {
    s = true;
    return it->second;
  }
}

std::string SaveController::getStr(const std::string& name, bool& s) const noexcept{
  //check if specified variable is stored.
  auto it = svars.find(name);
  if(it == svars.end()) {
    //not found in the map
    std::clog << "Error: save string variable '"<<name<<"' not found\n";
    s = false;
    return "";
  }
  else {
    s = true;
    return it->second;
  }
}
float SaveController::getFloat(const std::string& name, bool& s) const noexcept{
  //check if specified variable is stored.
  auto it = fvars.find(name);
  if(it == fvars.end()) {
    //not found in the map
    std::clog << "Error: Float variable '" << name << "' not found\n";
    s = false;
    return 0;
  }
  else {
    s = true;
    return it->second;
  }
}
bool SaveController::getBool(const std::string& name, bool& s) const noexcept {
  //check if specified variable is stored.
  auto it = bvars.find(name);
  if(it == bvars.end()) {
    //not found in the map
    std::clog << "Error: Bool variable '" << name << "' not found\n";
    s = false;
    return false;
  }
  else {
    s = true;
    return it->second;
  }
}


//excepting versions of the getters
int SaveController::getInt(const std::string& name) const {
  //check if specified variable is stored.
  auto it = ivars.find(name);
  if(it == ivars.end()) {
    //not found in the map
    throw std::out_of_range("SaveController::getInt() : Specified datum not found");
  }
  else {
    return it->second;
  }
}

std::string SaveController::getStr(const std::string& name) const {
  //check if specified variable is stored.
  auto it = svars.find(name);
  if(it == svars.end()) {
    //not found in the map
    std::clog << "Error: save string variable '"<<name<<"' not found\n";
    throw std::out_of_range("SaveController::getStr() : Specified datum not found");
  }
  else {
    return it->second;
  }
}
float SaveController::getFloat(const std::string& name) const {
  //check if specified variable is stored.
  auto it = fvars.find(name);
  if(it == fvars.end()) {
    //not found in the map
    throw std::out_of_range("SaveController::getFloat() : Specified datum not found");
    return 0;
  }
  else {
    return it->second;
  }
}
bool SaveController::getBool(const std::string& name) const {
  //check if specified variable is stored.
  auto it = bvars.find(name);
  if(it == bvars.end()) {
    //not found in the map
    throw std::out_of_range("SaveController::getBool() : Specified datum not found");
    return false;
  }
  else {
    return it->second;
  }
}
void SaveController::setStr(const std::string& name, const std::string& v) {
  if(!hasStr(name) && svars.size() == sMax) {
    std::cerr << "Error: string save limit exceeded\n";
    return;
  }
  if(v.size() > slMax) {
    //string variable is too long, abridge.
    svars[name] = v.substr(0, slMax);
  }
  else {
    svars[name] = v;
  }
}
void SaveController::setInt(const std::string& name, int v) {
  if(!hasInt(name) && ivars.size() == iMax) {
    std::cerr << "Error: string save limit exceeded\n";
    return;
  }
  ivars[name] = v;

}
void SaveController::setFloat(const std::string& name, float v) {
  if(!hasFloat(name) && fvars.size() == fMax) {
    std::cerr << "Error: string save limit exceeded\n";
    return;
  }
  fvars[name] = v;
}
void SaveController::setBool(const std::string& name, bool v) {
  if(!hasBool(name) && bvars.size() == bMax) {
    std::cerr << "Error: string save limit exceeded\n";
    return;
  }
  bvars[name] = v;
}


void SaveController::setSlot(const std::string& s) {
  saveSlot = s;
}
