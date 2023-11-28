#include "object.h"
#include <iostream>


Interface::Interface(sf::Vector2i p, std::string mes, std::string cut) : pos{p}, message{mes}, cutscene{cut} {}
Interface::Interface(sf::Vector2i p, std::string mes, std::string cut, std::vector<Object> obj) : pos{p}, message{mes}, cutscene{cut}, objs{obj} {}

Interface::Interface() : pos{0,0} {}

bool Object::getActive() const {
  return active;
}

int Object::getId() const{
  return id;
}
void Object::setId(int n) {
  id = n;
}

int Object::getLinkID() const {
  return link_id;
}
void Object::setLinkID(int i) {
  link_id = i;
}

int Object::getTextureID() const {
  return texture_id;
}
void Object::setTextureID(int i) {
  texture_id = i;
}

int Object::getParentID() const {
  return parent_id;
}
void Object::setParentID(int i) {
  parent_id = i;
}

int Object::getUID() const {
  return unique_id;
}

//deprecated
int Object::getValue() const{
  std::clog << "Object::getValue() is deprecated\n";
  return value;
}
void Object::setValue(int n) {
  std::clog << "Object::setValue() is deprecated\n";
  value = n;
}

void Object::setArg(std::size_t slot, int v) {
  if(slot > args.size()) {
    std::cerr << "Error: out-of-bounds object argument assignment\n";
    return;
  }
  args[slot] = v;
}

int Object::getArg(std::size_t slot) const {
  if(slot > args.size()) {
    std::cerr << "Error: out-of-bounds object argument fetch\n";
    return 0;
  }
  return args[slot];
}

std::array<int, 8> Object::getArgs() const {
  return args;
}
int Object::getStatus() const {
  return status;
}
std::string Object::getText() const {
  return text;
}
void Object::setArgs(std::array<int, 8> a) {
  args = a;
}
void Object::setSwitches(std::array<int, 8> s) {
  switches = s;
}
void Object::setText(const std::string& n) {
  text = n;
}
Object::Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt) : text{txt}, unique_id{-1} {
  std::clog << "This constructor variant is deprecated\n";
  pos.x = x;
  pos.y = y;
  size.x = wid;
  size.y = hei;
  id = i;
  value = v;
  solid = sol;
}
Object::Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a) : text{txt}, unique_id{-1} {
  std::clog << "This constructor variant is deprecated\n";
  pos.x = x;
  pos.y = y;
  size.x = wid;
  size.y = hei;
  id = i;
  value = v;
  solid = sol;
  args = a;
  active = true;
}
Object::Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a, int uid) : text{txt}, unique_id{uid} {
  pos.x = x;
  pos.y = y;
  size.x = wid;
  size.y = hei;
  id = i;
  value = v;
  solid = sol;
  args = a;
  active = true;
}

Object::Object() : unique_id{-1} {
  pos.x = 0;
  pos.y = 0;
  size.x = 0;
  size.y = 0;
  id = 0;
  solid = false;
  active = true;
}

Object::Object(int uid) : unique_id{uid} {
  pos.x = 0;
  pos.y = 0;
  size.x = 0;
  size.y = 0;
  id = 0;
  solid = false;
  active = true;
}

Interface Object::interact(Player*, Field*, SwitchHandler*) {
  // do absolutely nothing by default
  return Interface(pos, "", "");
}

Interface Object::interact(Object*, Field*, SwitchHandler*) {
  //do absolutely nothing by default
  return Interface(pos, "", "");
}

Interface Object::behave(SwitchHandler*) {
  // do absolutely nothing by default
  return Interface(pos, "", "");
}

CacheNodeAttributes Object::draw(const TextureCache* cache) {
  // draw a blank tile with no transforms
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("null");
  //no transforms, so tlist is left blank
  return cna;
}

bool Object::verify() {
  return true;
  //base object doesn't have any invalid configurations
}


void Object::notify(msg m) {
  //base objects do nothing upon being notified
  //only parents need it at the moment
  return;
}


// Deprecated
int binCat(std::uint16_t n, std::uint16_t o) {
  std::clog << "binCat() is deprecated\n";
  return (n << 16) | o;
}
Vector2<std::uint16_t> binDecat(int n) {
  std::clog << "binDecat() is deprecated\n";
  return Vector2<std::uint16_t>((n >> 16) & 0xffff, n & 0xffff);
}
