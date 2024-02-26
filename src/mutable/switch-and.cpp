#include "mutable/switch-and.h"

Interface SwAnd::behave(SwitchHandler* sh, Utility*) {
  sh->write(switches[SW::A], (sh->read(switches[SW::C]) && sh->read(switches[SW::D])));
  return Interface();
}
SwAnd::SwAnd(int uid) : Object{uid} {}
