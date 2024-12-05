#include "cutscene/cutscene.h"

Event Cutscene::getEvent(unsigned pos) {
  if(pos < eventList.size()) {
    return eventList[pos];
  }
  else {
    throw std::out_of_range("Cutscene::getEvent: in cutscene '" + name + "' (size is "+std::to_string(eventList.size())+", requested index is "+std::to_string(pos)+")");
  }
}
unsigned Cutscene::getListLen() {
  return eventList.size();
}

bool Cutscene::loadFromFile(const json11::Json& ob, std::string& fn) {
  //should have a name
  if(!ob["name"].is_string() || !ob["events"].is_array()) {
    std::cerr << "Couldn't parse cutscene\n";
    return false;
  }
  fn = ob["name"].string_value();
  //now, iterate through ob["events"] and call parse_json_event

  auto events = ob["events"].array_items();
  for(auto x : events) {
    Event e;
    if(!parse_json_event(x, e)) {
      std::cerr << "Couldn't parse event\n";
      return false;
    }
    eventList.push_back(e);

  }

  return true;
}
