#ifndef CUTSCENEPLAYER_H
#define CUTSCENEPLAYER_H

#include <vector>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "misc/util.h"
#include "mapdata/message.h"
#include "mutable/player.h"
#include "mutable/object.h"
#include "mapdata/mode.h"
#include "level/level.h"
#include "audio/audio.h"
#include "camera/camera.h"

#include "cutscene/cutscene.h"
#include "cutscene/cutscene-manager.h"
#include "misc/interface.h"
#include "utility/switch-handler.h"


//! A class that handles the playing of cutscenes
/*! 
 *  Contains references to all the components it can affect
 */
class CutscenePlayer {
private:
  Cutscene cutscene; //!< Currently playing cutscene
  unsigned timer=0;
  unsigned pos=0;  //!< position in the cutscene
  bool waiting=false;
  
  //references to engine components:
  Player& pl;
  Message& me;
  Level& le;
  ModeSwitcher& ms;
  MusicPlayer& mp;
  SwitchHandler& sh;
  Camera& cam;
public:
  //! Doesn't do anything. I'm not sure what I intended it to do
  bool playCutscene(const std::string&);
  //! Loads specified cutscene
  void loadCutscene(const Cutscene&);
  //! Runs every frame and checks the status of the cutscene.
  /*!
   *  If the current event has finished cooling down, calls playEvent on next
   *  event in the list. Otherwise, decrements cooldown timer.
   *  Returns false if the cutscene is over, Otherwise returns true
   */
  bool updateCutscene();
  //! Plays next event; called by updateCutscene if current event is finished
  /*!
   *  This function executes the current event in the cutscene
   */
  bool playEvent();
  //! Basic constructor
  CutscenePlayer(Player& p, Message& m, Level& l, ModeSwitcher& mo, MusicPlayer& mu, SwitchHandler& s, Camera& c);

  CutsceneManager man;
};


#endif
