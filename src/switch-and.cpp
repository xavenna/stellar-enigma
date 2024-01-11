#include "switch-and.h"

Interface SwAnd::behave(SwitchHandler* sh) {
  sh->write(switches[SW::Act1], (sh->read(switches[SW::Trig1]) && sh->read(switches[SW::Trig2])));
}
SwAnd::SwAnd(int uid) : Object{uid} {}
SwAnd::SwAnd(Object ob) : Object{ob} {}
