#include "mutable/cutplay.h"


CutPlay::CutPlay(int uid) : Object(uid) {
  vars[0] = 0;
}

Interface CutPlay::interact(Player*, Field*, SwitchHandler*) {
  // play cutscene, nothing else (for now)
  Interface inter;
  if(!vars[0]) {
    if(args[0]) { 
      status = Destroy;
    }
    else {
      status = Inactive;
    }
    
    vars[0] = args[1];
    inter.playCutscene(text);
  }
  return inter;
}

Interface CutPlay::behave(SwitchHandler*, Utility*) {
  if(vars[0] > 0) {
    vars[0]--;
  }
  if(!vars[0]) {
    status = Normal;
  }
  return Interface();
}
