#ifndef MAPDATA_H
#define MAPDATA_H

#include "player.h"
#include "mode.h"
#include "level.h"
#include "menu.h"
#include "audio.h"
#include "message.h"
#include "cutscene-manager.h"
#include "switch-handler.h"
#include "utility.h"
#include "interface-manager.h"
#include "texture-cache.h"
#include "save.h"
//! Mapdata is the class that encapsulates all level components
/*!
 * Provides a convenient encapsulation of all level components.
 * All helper components will be incorporated in eventually
 */
class MapData {
public:
  Player player; //!< The player - the user-game interface
  ModeSwitcher modeSwitcher; //!< The component that handles input, gamemode
  Level levelSlot; //!< Contains the Level data, manages interaction & collision
  Menu mainMenu; //!< Controls Menus and Splash screens
  MusicPlayer musicPlayer; //!< Controls playing of sounds and music
  Message message; //!< Displays textual information to the player
  CutscenePlayer cutscenePlayer; //!< Plays cutscenes to the player
  SwitchHandler switchHandler;  //!< Handles Switches
  Utility utility; //!< A utility provider, contains miscellaneous useful functions
  SaveController save; //!< Savedata manager

  //helper components
  TextureCache cache; //!< Manages textures & transforms
  InterfaceManager interface; //!< Controls the UI (Border & Panel
  

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
  //! Event handler wrapper function
  int handleEvents();
  //! Wraps up the frame, executed once at the end. Handles drawing
  void finishFrame(sf::RenderWindow&);
  //! Polls events
  void pollEvents(sf::RenderWindow&);
  //! Gets the current frame number
  unsigned long getFrameCount() const;
  //! The contructor takes several parameters for the members
  MapData(unsigned pCool, unsigned mWid, unsigned mCool, unsigned mElem, unsigned mCharSize);
private:
  unsigned long frameCount=0;
};
#endif
