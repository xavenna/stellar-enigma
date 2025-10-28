#include "cutscene/event.h"

std::string Event::getText() {
  return text;
}

Event::Type Event::getType() {
  return type;
}
int Event::getArg(unsigned index) {
  if(index > 8) {
    //error: 
    throw std::out_of_range("Event::getArg(int) : index is out of range");
  }
  return arguments[index];
}

int Event::operator[](unsigned index) {
  return getArg(index);
}
unsigned Event::getDuration() {
  return duration;
}

Event::Event(Event::Type ty, unsigned dur, const std::array<int, 8>& arg, const std::string& txt) : type{ty}, duration{dur}, arguments{arg}, text{txt} {
}
Event::Event() {
  
}
bool parse_json_event(const Json::Value& ob, Event& ev) {
  std::map<std::string, Event::Type> types = {
    {"update_node", Event::UpdateNode},
    {"write_switch", Event::WriteSwitch},
    {"message", Event::DisplayMessage},
    {"input", Event::GetInput},
    {"sound", Event::PlaySound},
    {"music", Event::SwitchMusic},
    {"move_player", Event::MovePlayer},
    {"move_object", Event::MoveObject},
    {"notify", Event::NotifyObject},
    {"menu", Event::Menu},
    {"label", Event::Label},
    {"map_load", Event::MapLoad},
    {"change_camera", Event::ChangeCamera},
  };
  if(!(ob["type"].isString() && ob["duration"].isNumeric())) {
    std::cerr << "Wrong fields\n";
    return false;
  }
  std::string type = ob["type"].asString();
  unsigned duration = ob["duration"].asFloat();
  std::string text = ob["text"].asString();

  std::array<int, 8> args;
  //get other args
  for(unsigned i=0;i<8;i++) {
    if(ob["arg_"+std::to_string(i)].isNumeric()) {
      args[i] = ob["arg_"+std::to_string(i)].asFloat();
    } else {
      args[i] = 0;
    }
  }

  
  if(types.find(type) == types.end()) {
    std::cerr << "Invalid event type '"<<type<<"'\n";
    return false;
  }
  ev = Event(types[type], duration, args, text);
  return true;
}
