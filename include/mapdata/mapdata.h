#ifndef MAPDATA_H
#define MAPDATA_H

#include "mutable/player.h"
#include "mapdata/mode.h"
#include "level/level.h"
#include "mapdata/menu.h"
#include "audio/audio.h"
#include "mapdata/message.h"
#include "cutscene/cutscene-manager.h"
#include "cutscene/cutsceneplayer.h"
#include "utility/switch-handler.h"
#include "utility/utility.h"
#include "interface/interface-manager.h"
#include "utility/texture-cache.h"
#include "utility/save.h"
//! Mapdata is the class that encapsulates all level components
/*!
 * Provides a convenient encapsulation of all level components.
 * All helper components will be incorporated in eventually
 */
class MapData {
public:
  //core components
  Player player; //!< The player - the user-game interface
  ModeSwitcher modeSwitcher; //!< The component that handles input, gamemode
  Level levelSlot; //!< Contains the Level data, manages interaction & collision
  Menu mainMenu; //!< Controls Menus and Splash screens
  MusicPlayer musicPlayer; //!< Controls playing of sounds and music
  Message message; //!< Displays textual information to the player
  CutscenePlayer cutscenePlayer; //!< Plays cutscenes to the player

  //move to utility
  SwitchHandler switchHandler;  //!< Handles Switches

  //helper components... maybe move texturecache to utility
  TextureCache cache; //!< Manages textures & transforms
  InterfaceManager interface; //!< Controls the UI (Border & Panel
  Utility utility; //!< A utility provider, contains miscellaneous useful functions
  

  //! Required size of the RenderWindow
  sf::Vector2i windowSize;

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
  //! Handles events if the engine is in mode 5
  void event5Handle();
  //! Event handler wrapper function
  int handleEvents();

  //! Handles interactions between objects and player
  void handleInteractions();
  //! Wraps up the frame, executed once at the end. Handles drawing
  void finishFrame(sf::RenderWindow&);
  //! Polls events
  void pollEvents(sf::RenderWindow&);
  //! Gets the current frame number
  unsigned long getFrameCount() const;
  //! load a level from file
  void loadLevel(const std::string&);
  //! The contructor takes several parameters for the members
  MapData(unsigned mWid, unsigned mCool, unsigned mElem, unsigned mCharSize);
private:
  unsigned long frameCount=0;
};
#endif
