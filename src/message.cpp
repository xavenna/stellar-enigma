#include "message.h"
#include <iostream>

// Find a way to eliminate message spam
//maybe a cooldown?


bool Message::getPrinted() const {
  return printed;
}
void Message::addMessage(const std::string& str) {
  if((printQueue.size() != 0 && printQueue.back() != str) || (printQueue.size() == 0 && message != str)) {
    printQueue.push_back(str);
  }
}
void Message::handleMessages() {
  if(!message.empty()) {
    //there is a message 
    //check if it is done displaying
    if(pointer == message.size() && printed == false) {
      //start cooldown
      printed = true;
      cooldown = maxCool;
    }
    else if(pointer < message.size() && printed == false) {
      //add additional character to display
      formMessage += message[pointer];
      pointer++;  //I sure hope this works
    }
    else if(printed == true && cooldown == 0) {
      //message is done, clear message
      message.clear();
      formMessage.clear();
      printed = false;
      pointer = 0;

    }
    else {
      cooldown--;
    }
  }
  else {
    //there is no current message
    //check if there are messages in the queue
    if(printQueue.size() == 0) {
      //do nothing, as queue is empty
      return;
    }
    else {
      message = printQueue.front();
      formMessage.reserve(message.size());  //avoids reallocations later
      printQueue.pop_front();
      printed = false;
    }
  }
}
void Message::wrapMessage() {
  //wraps message to width, and sets it to text string
  std::string out;
  std::string n = formMessage;  //the raw message
  size_t pads = (width-(n.size()%width)%width);  //
  for(unsigned i=0;i<pads;i++)
    n += ' ';  //pad the message so it is an even multiple of width
  for(unsigned i=0;i<unsigned(n.size()/width)+1;i++) {
    out += n.substr(i*width,width) + '\n';
  }
  setString(out);
}
Message::Message(unsigned wid, unsigned cool, unsigned elem, unsigned charSize) : width{wid}, maxCool{cool}, maxElem{elem} {
  printed = false;
  pointer = 0;
  setString("");
  setFillColor(sf::Color::White);
  setCharacterSize(charSize);
  courier.loadFromFile("assets/cour.ttf");
  setFont(courier);
}

//this is just for debugging, and is not needed in the final thing
std::string Message::getMessage() {
  return message;
}

int Message::getSize() {
  return printQueue.size();
}
