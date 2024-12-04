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
int Event::getDuration() {
  return duration;
}

Event::Event(Event::Type ty, int dur, const std::array<int, 8>& arg, const std::string& txt) : type{ty}, duration{dur}, arguments{arg}, text{txt} {
}
Event::Event() {
  
}
bool parse_json_event(const json11::Json& ob, Event& ev) {
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
    {"map_load", Event::MapLoad}
  };
  if(!(ob["type"].is_string() && ob["duration"].is_number())) {
    std::cerr << "Wrong fields\n";
    return false;
  }
  std::string type = ob["type"].string_value();
  unsigned duration = ob["duration"].number_value();
  std::string text = ob["text"].string_value();

  std::array<int, 8> args;
  //get other args
  for(unsigned i=0;i<8;i++) {
    if(ob["arg_"+std::to_string(i)].is_number()) {
      args[i] = ob["arg_"+std::to_string(i)].number_value();
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

//deprecated
bool line2event(const std::string& line, Event& ev) {
  //parse line, somehow
  //first, split the line into a list of strings
  std::vector<std::string> fieldList;
  std::string key = "`";
  parse(line, fieldList, key);
  std::string t;
  int du{0};
  Event::Type et{Event::Invalid};
  std::array<int, 8> ar;
  //now, iterate through the fields and parse them
  for(size_t i=0;i<fieldList.size();i++) {
    if(i==0) {
      if(isNum(fieldList[0])) {
	et = static_cast<Event::Type>(std::stoi(fieldList[i]));
	//cast this because int -> enum is narrowing
      }
      else {
	std::cout << "Error: non-numeric value in numeric field.\n";
	return false;
      }
    }
    else if(i==1) {
      if(isNum(fieldList[1])) {
        du = std::stoi(fieldList[i]);
      }
      else {
	std::cout << "Error: non-numeric value in numeric field.\n";
	return false;
      }
    }
    else if(i > 1 && i <= 9) {
      if(isNum(fieldList[i])) {
	ar[i-2] = std::stoi(fieldList[i]);
      }
      else {
	std::cout << "Error: non-numeric value in numeric field.\n";
	return false;
      }
    }
    else {
      t = fieldList[i];
    }
  }
  Event e(et, du, ar, t);
  ev = e;
  return true;
}
/*event components:
 * Type (enum, stored as int)
 * duration (int)
 * args (int[8])
 * text (char*) (until end of line)
 

Line:
type`dur`a0`a1`a2`a3`a4`a5`a6`a7`text
 */
