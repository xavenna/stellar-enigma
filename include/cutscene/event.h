#ifndef EVENT_H
#define EVENT_H

#include <array>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

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
    */
    // New Event list

    UpdateNode = 0,     //!< Updates a map node
    WriteSwitch = 1,    //!< Writes a value to a switch
    DisplayMessage = 2, //!< Prints a message in the messagebox
    GetInput = 3,       //!< Waits for player input
    PlaySound = 4,      //!< Plays a sound effect
    SwitchMusic = 5,    //!< Switches music to specified file
    MovePlayer = 6,     //!< Moves player
    MoveObject = 7,     //!< Moves an object
    NotifyObject = 8,   //!< Sends message to object
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
  int getDuration();
  //! Constructs event with specified attributes
  Event(Event::Type ty, int dur, const std::array<int, 8>& arg, const std::string& txt);
  //! Constructs an empty event
  Event();
private:
  Type type;  //!< Type of event
  int duration; //!< Duration in frames of the event
  std::array<int, 8> arguments;  //!< arguments for the event
  //The 8 here is purely arbitrary, and may change
  std::string text;  //!< text argument
};

//! Sets attributes of passed event using passed textual event representation
/*!
 *  If it fails, returns false. Doesn't empty event (add this in the future)
 */
bool line2event(const std::string&, Event&);

#endif
