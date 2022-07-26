#include "message.h"
#include <iostream>

bool Message::getPrinted() const {
  return printed;
}
void Message::addMessage(const std::string& str) {
  if(printQueue.size() != maxElem) {
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
  for(size_t i=0;i<pads;i++)
    n += ' ';  //pad the message so it is an even multiple of width
  for(int i=0;i<int(n.size()/width)+1;i++) {
    out += n.substr(i*width,width) + '\n';
  }
  text.setString(out);
}
Message::Message(size_t n, size_t o, size_t p) : width{n}, maxCool{o}, maxElem{p} {
  text.setString("");
  printed = false;
  pointer = 0;
}

//this is just for debugging, and is not needed in the final thing
std::string Message::getMessage() {
  return message;
}

int Message::getSize() {
  return printQueue.size();
}
