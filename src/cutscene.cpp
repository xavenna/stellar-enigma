#include "cutscene.h"

Event Cutscene::getEvent(unsigned pos) {
  return eventList.at(pos);
}
int Cutscene::getListLen() {
  return eventList.size();
}

bool Cutscene::loadFromFile(const std::string& filename) {
  eventList.clear();
  name.erase();
  std::ifstream cutLoad;
  std::string line;
  Event tempEv;
  unsigned counter = 0;
  cutLoad.open(filename);
  if(!cutLoad.is_open()) {
    std::cout << "Error: cutscene file could not be found.\n";
    return false;
  }
  while(cutLoad.peek() != EOF) {
    std::getline(cutLoad, line);
    //attempt to transform line into a cutscene
    // empty lines and lines beginning with a pound (comments) are ignored
    if(line.size() > 0 && line[0] != '#' && !line2event(line, tempEv)) {
      //event was not valid
      std::cout << "Error at line " << counter << ": Event is invalid\n";
      return false;
    }
    //append event to list
    eventList.push_back(tempEv);
    counter++;
  }
  //set cutscene name
  name = filename.substr(0, filename.find("."));
  return true;
}
