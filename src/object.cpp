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

int Object::getValue() const{
  return value;
}
void Object::setValue(int n) {
  value = n;
}

void Object::setArg(std::size_t slot, int value) {
  if(slot > args.size()) {
    std::cerr << "Error: out-of-bounds object argument assignment\n";
    return;
  }
  args[slot] = value;
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
void Object::setText(const std::string& n) {
  text = n;
}
Object::Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt) : text{txt} {
  pos.x = x;
  pos.y = y;
  size.x = wid;
  size.y = hei;
  id = i;
  value = v;
  solid = sol;
}
Object::Object(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a) : text{txt} {
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

Object::Object() {
  pos.x = 0;
  pos.y = 0;
  size.x = 0;
  size.y = 0;
  id = 0;
  solid = false;
  active = true;
}


Interface Object::interact(Player*, Field*, bool) {
  // do absolutely nothing by default
  return Interface(pos, "", "");
}

Interface Object::interact(Object*, Field*, bool) {
  //do absolutely nothing by default
  return Interface(pos, "", "");
}

Interface Object::behave() {
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


int binCat(std::uint16_t n, std::uint16_t o) {
  return (n << 16) | o;
}
Vector2<std::uint16_t> binDecat(int n) {
  return Vector2<std::uint16_t>((n >> 16) & 0xffff, n & 0xffff);
}
