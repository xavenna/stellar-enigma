#include "interface.h"
#include "object.h"




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
