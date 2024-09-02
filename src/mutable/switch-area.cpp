#include "mutable/switch-area.h"

Interface SwArea::interact(Object* o, Field*, SwitchHandler*) {
  // play cutscene, nothing else (for now)
  Interface inter;
  if(o->Type() == Object::Play) {
    if(args[0]) { 
      status = Destroy;
    }
    else {
      status = Inactive;
    }
    occupied = true;
  }
  return inter;
}

Interface SwArea::behave(SwitchHandler* sh, Utility*) {
  sh->write(switches[SW::A], occupied);
  occupied = false;
  return Interface();
}

SwArea::SwArea(int uid) : Object{uid} {};
