#ifndef STELLAR_ENIGMA_HPP
#define STELLAR_ENIGMA_HPP
#include <iostream>
#include <vector>
#include <deque>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "direction.h"
#include "mutable.h"
#include "object.h"
#include "entity.h"
#include "player.h"
#include "message.h"
#include "mapnode.h"
#include "texturemap.h"
#include "level.h"
#include "cutsceneplayer.h"
#include "menu.h"

#include "mode.h"
#include "level-util.h"
#include "audio.h"

//Documented

// This is the header to include for stellar-enigma

//! Mapdata is the class that encapsulates all level components
/*!
 *  It is passed to the functions found in custom.cpp to make the code cleaner
 */
class MapData {
public:
  Player& player; //!< A reference to the player
  ModeSwitcher& modeSwitcher; //!< A reference to the mode switcher
  Level& levelSlot; //!< A reference to the level container
  Menu& mainMenu; //!< A reference to the main menu
  MusicPlayer& musicPlayer; //!< A reference to the music player
  Message& message;
  //! The constructor, takes pointers to the members and initializes the references with them
  MapData(Player*, ModeSwitcher*, Level*, Menu*, MusicPlayer*, Message*);
};


//these have to be defined here to prevent a circular include loop

//maybe some of these should be encapsulated into MapData

//! Initializes window features, specifically title and size
void initialSetup(std::string&, int&, int&, int&);
//! Any custom initialization behavior goes here
void customInit(MapData&);  
//! Handles events if the engine is in mode 0
void event0Handle(MapData&);
//! Handles events if the engine is in mode 1
void event1Handle(MapData&);
//! Handles events if the engine is in mode 2
void event2Handle(MapData&);
//! Handles events if the engine is in mode 3
void event3Handle(MapData&);
//! Handles events if the engine is in mode 4
void event4Handle(MapData&);
#endif
