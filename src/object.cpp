#include "object.h"
#include <iostream>



Interface::Interface() {}



void Interface::addMessage(const std::string& m) {
  message.push_back(m);
}
void Interface::playCutscene(const std::string& c) {
  cutscene.push_back(c);
}
void Interface::playSound(const std::string& s) {
  sounds.push_back(s);
}
void Interface::spawnObject(Object o, const std::string& s) {
  objs.push_back(std::pair(o, s));
}

void Interface::notify(msg m) {
  notifications.push_back(m);
}

void Interface::clearObjs() {
  objs.clear();
}

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
std::array<int, 8> Object::getSwitches() const {
  return switches;
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
void Object::setSwitch(unsigned index, int value) {
  if(index >= 8) {
    std::clog << "Error: out-of-bounds switch assignment. Ignored\n";
    return;
  }
  switches[index] = value;
}
void Object::setText(const std::string& n) {
  text = n;
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

Object::Object(int uid) : unique_id{uid} {
  Object();
  active = true;
}


Interface Object::interact(Player*, Field*, SwitchHandler*) {
  // do absolutely nothing by default
  return Interface();
}

Interface Object::interact(Object*, Field*, SwitchHandler*) {
  //do absolutely nothing by default
  return Interface();
}

Interface Object::behave(SwitchHandler*) {
  // do absolutely nothing by default
  return Interface();
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
