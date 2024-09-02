#include "mutable/switch-and.h"

Interface SwAnd::behave(SwitchHandler* sh, Utility*) {
  sh->write(switches[SW::A], (sh->read(switches[SW::C]) && sh->read(switches[SW::D])));
  return Interface();
}

bool SwAnd::verify() {
  if(switches[SW::A] < 0 || switches[SW::A] > 255 || switches[SW::C] < 0 || 
      switches[SW::C] > 255 || switches[SW::D] < 0 || switches[SW::D] > 255) {
    //out-of-bounds switch
    return false;
  }
  return true;
}
SwAnd::SwAnd(int uid) : Object{uid} {}
