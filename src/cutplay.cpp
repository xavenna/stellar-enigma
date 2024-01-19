#include "cutplay.h"


CutPlay::CutPlay(int uid) : Object(uid) {
  vars[0] = 0;
  args[1] = 30;
}

Interface CutPlay::interact(Player*, Field*, SwitchHandler*) {
  // play cutscene, nothing else (for now)
  Interface inter;
  if(!vars[0]) {
    if(args[0]) { 
      status = Destroy;
    }
    else {
      status = Normal;
    }

    vars[0] = args[1];
    active = false;
    inter.playCutscene(text);
  }
  return inter;
}

Interface CutPlay::behave(SwitchHandler*) {
  if(vars[0] > 0) {
    vars[0]--;
  }
  if(!vars[0]) {
    active = true;
  }
  return Interface();
}
