#ifndef EVENT_H
#define EVENT_H

#include <array>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <json11.hpp>

#include "misc/util.h"

//! An event in a cutscene
/*!
 *  Contains the event type and event duration, as well as 8 integer arguments and one string
 *  argument.
 */
class Event {
public:
  //! Types of events that can be part of a cutscene
  enum Type {
    /*
    PlayerUpdate = 0,  //!< Updates the player
    ObjectPlace = 2,   //!< Places an object
    ObjectRemove = 3,  //!< Removes an object
    ImageDisplay = 5,  //!< Displays an image
    */
    // New Event list

    UpdateNode = 0,     //!< Updates a map tile
    WriteSwitch = 1,    //!< Writes a value to a switch
    DisplayMessage = 2, //!< Prints a message in the messagebox
    GetInput = 3,       //!< Waits for player input
    PlaySound = 4,      //!< Plays a sound effect
    SwitchMusic = 5,    //!< Switches music to specified file
    MovePlayer = 6,     //!< Moves player
    MoveObject = 7,     //!< Moves an object
    NotifyObject = 8,   //!< Sends message to object
    Menu = 9,           //!< A dialogue choice.
    Label = 10,         //!< A jump point for a menu
    MapLoad = 11,       //!< Loads a new map?
    ChangeCamera = 12,  //!< Switches to new camera config

    /*
     * Events to implement eventually:
    DisplayImage,
    */
    Invalid = -1       //!< Invalid Type
  };
  //! Returns event type
  Event::Type getType();
  //! Returns text argument
  std::string getText();
  //! Returns specified argument from internal argument vector
  int getArg(unsigned);
  //! A wrapper around getArg
  int operator[](unsigned);
  //! Returns duration of event
  unsigned getDuration();
  //! Constructs event with specified attributes
  Event(Event::Type ty, unsigned dur, const std::array<int, 8>& arg, const std::string& txt);
  //! Constructs an empty event
  Event();
private:
  Type type;  //!< Type of event
  unsigned duration; //!< Duration in frames of the event
  std::array<int, 8> arguments;  //!< arguments for the event
  //The 8 here is purely arbitrary, and may change
  std::string text;  //!< text argument
};

//! Sets attributes of passed event using passed textual event representation
/*!
 *  If it fails, returns false. Doesn't empty event (add this in the future)
 *  Deprecated
 */
bool line2event(const std::string&, Event&);
bool parse_json_event(const json11::Json&, Event&);

#endif
