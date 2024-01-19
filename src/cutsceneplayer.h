#ifndef CUTSCENEPLAYER_H
#define CUTSCENEPLAYER_H

#include <vector>
#include <SFML/Graphics.hpp>

#include "util.h"
#include "message.h"
#include "player.h"
#include "object.h"
#include "mode.h"
#include "level.h"
#include "audio.h"

#include "cutscene.h"
#include "cutscene-manager.h"


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
  bool updateCutscene(Player&, Message&, Level&, ModeSwitcher&, MusicPlayer&);
  //! Plays next event; called by updateCutscene if current event is finished
  /*!
   *  This function executes the current event in the cutscene
   */
  bool playEvent(Player&, Message&, Level&, MusicPlayer&);
  //! Basic constructor
  CutscenePlayer();

  CutsceneManager man;
};


#endif
