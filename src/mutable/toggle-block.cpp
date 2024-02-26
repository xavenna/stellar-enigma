#include "mutable/toggle-block.h"
#include <iostream>


ToggleBlock::ToggleBlock(int uid) : Solid(uid) {
  vars[0] = args[0];
  vars[1] = 0;
}

Interface ToggleBlock::interact(Player* p, Field* f, SwitchHandler* s) {
  Interface i = Solid::interact(p, f, s);
  if(interacting) {
    //toggle state
    if(!vars[1]) {
      vars[0] = !vars[0];
      vars[1] = args[1];
    }

  }
  return i;
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

Interface ToggleBlock::behave(SwitchHandler*, Utility*) {
  if(vars[1] > 0) {
    vars[1]--;
  }
  //this should toggle a switch
  return Interface();
}
