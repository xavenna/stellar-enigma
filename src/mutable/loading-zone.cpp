#include "mutable/loading-zone.h"

LoadingZone::LoadingZone(int uid) : Object{uid} {
  cooldown = 0;
}

Interface LoadingZone::interact(Object* o, Field*, SwitchHandler*) {
  Interface inter;
  if(o->Type() == Object::Play) {
    if(args[0] == 0 && !cooldown) {
      cooldown = args[3];
      //trigger a camera change
      
      //args[1] indicates if mode4 is used or not.
      inter.loadLevel(text, args[1]);
      if(args[2]) { 
        status = Destroy;
      }
      else {
        status = Inactive;
      }
    }
  }
  return inter;
}


Interface LoadingZone::behave(SwitchHandler* sh, Utility*) {
  Interface inter;
  if(waiting) { //waiting for switch to deactivate
    waiting = sh->read(switches[SW::A]);
  }

  if(args[1] == 1 && sh->read(switches[SW::A]) == 1 && !cooldown && !waiting) {
    cooldown = args[3];
    inter.loadLevel(text, args[1]);
    if(args[2]) { 
      status = Destroy;
    }
    else {
      status = Inactive;
    }
    waiting=true;
  }
  if(cooldown > 0 && !waiting) {
    cooldown--;
  }
  if(!cooldown) {
    status = Normal;
  }
  return inter;
}

bool LoadingZone::verify() {
  if(args[0] == 1 || args[0] == 0) {
    return true;
  }
  if(args[1] == 1 || args[1] == 0) {
    return true;
  }
  if(args[2] == 1 || args[2] == 0) {
    return true;
  }
  if(args[3] >= 0) {
    return true;
  }
  if(switches[SW::A] >= 0) {
    return true;
  }
  return false;
}
