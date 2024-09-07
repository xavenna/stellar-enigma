#include "mutable/toggle-block.h"
#include <iostream>


ToggleBlock::ToggleBlock(int uid) : Solid(uid) {
  vars[0] = args[0];
  vars[1] = 0;
}

Interface ToggleBlock::interact(Object* p, Field* f, SwitchHandler* s) {
  Interface i = Solid::interact(p, f, s);
  /*
  if(p->Type() == Object::Play) {
    //toggle state
    if(!vars[1]) {
      vars[0] = !vars[0];
      vars[1] = args[1];
    }
  }
  */
  return i;
}

bool ToggleBlock::use(Player*) {
  //toggle state
  if(!vars[1]) {
    vars[0] = !vars[0];
    vars[1] = args[1];
  }
  return false;
}

CacheNodeAttributes ToggleBlock::draw(const TextureCache* cache) {
  // check value of some internal state, and draw using obj_args
  CacheNodeAttributes cna;
  if(vars[0] == 1) {
    cna.srcImg = cache->reverseHash("on-barr-big");
  }
  else {
    cna.srcImg = cache->reverseHash("off-barr-big");
  }

  //no transforms, so tlist is left blank
  return cna;
}

Interface ToggleBlock::behave(SwitchHandler* sh, Utility*) {
  if(vars[1] > 0) {
    vars[1]--;
  }
  //this should toggle a switch
  sh->write(switches[SW::A], vars[0]);
  return Interface();
}

bool ToggleBlock::verify() {
  if(switches[SW::A] < 0 || switches[SW::A] > 255) {
    return false;
  }
  if(args[1] < 0) {
    return false;
  }

  return true;
}
