#ifndef EVENT_H
#define EVENT_H

#include <array>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "util.h"

//! An event in a cutscene
/*!
 *  Contains the event type and event duration, as well as 8 integer arguments and one string
 *  argument.
 */
class Event {
public:
  //! A enum of types of events.
  enum Type {
    PlayerUpdate = 0,  //!< Updates the player
    EntityMove = 1,    //!< Moves an entity
    ObjectPlace = 2,   //!< Places an object
    ObjectRemove = 3,  //!< Removes an object
    MessageDisplay = 4,//!< Displays a message
    ImageDisplay = 5,  //!< Displays an image
    GetInput = 6,      //!< Waits for player input
    NodeUpdate = 7,    //!< Updates a node on the map
    SoundPlay = 8,     //!< Plays a sound
    MusicPlay = 9,     //!< Plays specified music
    MapLoad = 10,      //!< Load a new Map
    Invalid = -1       //!< Invalid Type
  };
  //! Returns event type
  Event::Type getType();
  //! Returns text argument
  std::string getText();
  //! Returns specified argument from internal argument vector
  int getArg(int);
  //! A wrapper around getArg
  int operator[](int);
  //! Returns duration of event
  int getDuration();
  //! Constructs event with specified attributes
  Event(Event::Type ty, int dur, const std::array<int, 8>& arg, const std::string& txt);
  //! Constructs an empty event
  Event();
private:
  Type type;  //!< Type of event
  int duration; //!< Duration in frames of the event
  std::array<int, 8> arguments;  //!< arguments for the event
  std::string text;  //!< text argument
  //The 8 here is purely arbitrary, and may change
};

//! Sets attributes of passed event using passed textual event representation
/*!
 *  If it fails, returns false. Doesn't empty event (add this in the future)
 */
bool line2event(const std::string&, Event&);

#endif
