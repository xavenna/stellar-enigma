#ifndef CUTSCENEPLAYER_H
#define CUTSCENEPLAYER_H

#include <vector>
#include <SFML/Graphics.hpp>

#include "util.h"
#include "message.h"
#include "player.h"
#include "object.h"
#include "entity.h"
#include "mode.h"
#include "level.h"

#include "cutscene.h"


//! A class that will eventually handle the playing of cutscenes
/*! 
 *  It has not been written yet, but will be able to display video-style
 *  cutscenes, which have no player input, and interactive cutscenes, which
 *  can (at least) print messages which the player can respond to
 */
class CutscenePlayer {
private:
  Cutscene cutscene;
  int timer;
  unsigned pos;  //position in the cutscene
public:
  //! Doesn't do anything yet
  void playCutscene();
  //! Doesn't do anything yet
  void loadCutscene(const Cutscene&);
  //! This will be run every frame and update the cutscene
  /*!
   *  Returns false if the cutscene is over
   *  Returns true otherwise
   */
  bool updateCutscene(Player&, Message&, Level&, ModeSwitcher&);
  //! Plays next event; called by updateCutscene if current event is finished
  bool playEvent(Player&, Message&, Level&);
  CutscenePlayer();
};


#endif
