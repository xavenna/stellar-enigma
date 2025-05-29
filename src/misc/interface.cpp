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

void Interface::selectConfig(const std::string& c) {
  camera = c;
}

void Interface::loadLevel(const std::string& l, bool usem4) {
  useMode4 = usem4;
  level = l;
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
  t.camera = this->camera; //likewise
  t.level = this->level;   //...
  t.useMode4 = this->useMode4; //...
  return t;
}
