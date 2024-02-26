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

#include "cutscene/cutscene.h"
#include "cutscene/cutscene-manager.h"
#include "misc/interface.h"
#include "utility/switch-handler.h"


//! A class that handles the playing of cutscenes
/*! 
 *   
 */
class CutscenePlayer {
private:
  Cutscene cutscene; //!< Currently playing cutscene
  int timer;
  unsigned pos;  //!< position in the cutscene
public:
  //! Doesn't do anything. I'm not sure what I intended it to do
  void playCutscene();
  //! Loads specified cutscene
  void loadCutscene(const Cutscene&);
  //! Runs every frame and checks the status of the cutscene.
  /*!
   *  If the current event has finished cooling down, calls playEvent on next
   *  event in the list. Otherwise, decrements cooldown timer.
   *  Returns false if the cutscene is over, Otherwise returns true
   */
  bool updateCutscene(Player&, Message&, Level&, ModeSwitcher&, MusicPlayer&, SwitchHandler& sh);
  //! Plays next event; called by updateCutscene if current event is finished
  /*!
   *  This function executes the current event in the cutscene
   */
  bool playEvent(Player&, Message&, Level&, MusicPlayer&, SwitchHandler&);
  //! Basic constructor
  CutscenePlayer();

  CutsceneManager man;
};


#endif
