#include "event.h"

std::string Event::getText() {
  return text;
}

Event::Type Event::getType() {
  return type;
}
int Event::getArg(int index) {
  if(index > 8 || index < 0) {
    //error: 
    throw std::out_of_range("Event::getArg(int) : index is out of range");
  }
  return arguments[index];
}

int Event::operator[](int index) {
  return getArg(index);
}
int Event::getDuration() {
  return duration;
}

Event::Event(Event::Type ty, int dur, const std::array<int, 8>& arg, const std::string& txt) : type{ty}, duration{dur}, arguments{arg}, text{txt} {
}
Event::Event() {
  
}
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
