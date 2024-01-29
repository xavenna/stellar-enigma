#include "switch-and.h"

Interface SwAnd::behave(SwitchHandler* sh) {
  sh->write(switches[SW::A], (sh->read(switches[SW::In1]) && sh->read(switches[SW::In2])));
  return Interface();
}
SwAnd::SwAnd(int uid) : Object{uid} {}
