#ifndef MAPDATA_H
#define MAPDATA_H

#include "player.h"
#include "mode.h"
#include "level.h"
#include "menu.h"
#include "audio.h"
#include "message.h"
#include "cutscene-manager.h"
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
  Message& message; //!< A reference to the message displayer
  CutscenePlayer& cutscenePlayer; //!< A reference to the cutscene player
  CutsceneManager& cutsceneManager; //!< A reference to the cutscene manager
  //! Any custom initialization behavior goes here
  void customInit();  
  //! Handles events if the engine is in mode 0
  int event0Handle();
  //! Handles events if the engine is in mode 1
  void event1Handle();
  //! Handles events if the engine is in mode 2
  void event2Handle();
  //! Handles events if the engine is in mode 3
  void event3Handle();
  //! Handles events if the engine is in mode 4
  void event4Handle();
  //! The constructor, takes pointers to the members and initializes the references with them
  MapData(Player*, ModeSwitcher*, Level*, Menu*, MusicPlayer*, Message*, CutscenePlayer*, CutsceneManager*);
};
#endif
