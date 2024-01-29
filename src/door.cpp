#include "door.h"

Door::Door(int uid) : Solid(uid) {
  vars[0] = true;
  vars[1] = false;
}


Interface Door::interact(Object* p, Field* f, SwitchHandler* s) {
  if(!vars[0]) {
    return Interface();
  }
  //this prevents having to copy-paste the code from Solid to here
  return Solid::interact(p, f, s);
}

Interface Door::interact(Player* p, Field* f, SwitchHandler* s) {
  if(!vars[0]) {
    return Interface();
  }
  return Solid::interact(p, f, s);
}


CacheNodeAttributes Door::draw(const TextureCache* cache) {
  // draw a solid object with no transforms
  // use obj_arg[0] to decide which texture to draw
  CacheNodeAttributes cna;
  switch(texture_id) {
  default:
    cna.srcImg = cache->reverseHash("crate");
  //other textures can be placed here for more design freedom
  }
  //no transforms, so tlist is left blank
  return cna;
}

bool Door::verify() {
  //SW::In1 should be a valid switch.
  return (switches[SW::In1] >= 0 && switches[SW::In1] < 256);
}

Interface Door::behave(SwitchHandler* sh) {
  //poll SW_Trig1
  bool sw = sh->read(switches[SW::In1]);
  if(!vars[1]) {
    if(sw) {
      vars[1] = true;
      vars[0] = !vars[0];
      if(vars[0]) {
        status = Normal;
      }
      else {
        status = Inactive;
      }

    }
  }
  else {
    if(!sw) {
      vars[1] = false;
    }
  }
  return Interface();
}
