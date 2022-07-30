#ifndef EVENT_H
#define EVENT_H

#include <array>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "util.h"

//! An event in a cutscene
class Event {
public:
  //! A enum of types of events.
  enum Type {
    PlayerMove = 0,    //!< Moves the player
    EntityMove = 1,    //!< Moves an entity
    ObjectPlace = 2,   //!< Places an object
    ObjectRemove = 3,  //!< Removes an object
    MessageDisplay = 4,//!< Displays a message
    ImageDisplay = 5,  //!< Displays an image
    GetInput = 6       //!< Waits for player input
  };
  Event::Type getType();
  std::string getText();
  int getArg(int);  //I may use operator[] for this as well; that would be cool
  int getDuration();
  //add constructor that takes all thingies as args
  Event(Event::Type ty, int dur, const std::array<int, 8>& arg, const std::string& txt);
  Event();
private:
  Type type;  //!< Type of event
  int duration; //!< Duration in frames of the event
  std::array<int, 8> arguments;  //!< arguments for the event
  std::string text;  //!< text argument
  //The 8 here is purely arbitrary, and may change
};

bool line2event(const std::string&, Event&);

#endif
