#include "misc/interface.h"
#include "mutable/object.h"




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

void Interface::openMenu(const std::string& s) {
  menu = s;
}


Interface Interface::operator+(const Interface& i) {
  Interface t;
  t.message = vecCat(this->message, i.message);
  t.cutscene = vecCat(this->cutscene, i.cutscene);
  t.sounds = vecCat(this->sounds, i.sounds);

  //vecCat doesn't work here... We have to use an alternative approach
  t.objs.reserve(this->objs.size() + i.objs.size());

  for(auto x : this->objs) {
    t.objs.push_back(x);
  }
  for(auto x : i.objs) {
    t.objs.push_back(x);
  }


  t.menu = this->menu;  //First Interface takes priority
  return t;
}
