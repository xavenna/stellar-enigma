#include "mutable/object.h"
#include "mutable/player.h"
#include <iostream>


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
Object::Status Object::getStatus() const {
  return status;
}
void Object::setStatus(Object::Status s) {
  status = s;
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
  solid = false;
  status = Object::Normal;
  mass = 1.f;
  selfPush = zero2<float>();
}

Object::Object(int uid) : unique_id{uid} {
  Object();
}

void Object::setPushback(Direction d) {
  pushback.set(d, true);
}
void Object::resetPushback() {
  pushback.set(Up, false);
  pushback.set(Down, false);
  pushback.set(Left, false);
  pushback.set(Right, false);
}

bool Object::readPushback(Direction d) {
  return pushback[d];
}

Interface Object::interact(Object*, Field*, SwitchHandler*) {
  //do absolutely nothing by default
  return Interface();
}

Interface Object::behave(SwitchHandler*, Utility*) {
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


void Object::notify(msg) {
  //base objects do nothing upon being notified
  //only parents need it at the moment
  return;
}


bool Object::use(Player* p) {
  return false;
}
