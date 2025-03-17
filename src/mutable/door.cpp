#include "mutable/door.h"

Door::Door(int uid) : Solid(uid) {
  locked = true;
  cooldown = false;
}


Interface Door::interact(Object* p, Field* f, SwitchHandler* s) {
  if(!locked) {
    return Interface();
  }
  //this prevents having to copy-paste the code from Solid to here
  return Solid::interact(p, f, s);
}

CacheNodeAttributes Door::draw(const TextureCache* cache) {
  // draw a solid object with no transforms
  // use obj_arg[0] to decide which texture to draw
  CacheNodeAttributes cna;
  switch(texture_id) {
  default:
    if(!locked) {
      cna.srcImg = "crate";
    }
    else {
      cna.srcImg = "locked-door";
    }
  //other textures can be placed here for more design freedom
  }
  //no transforms, so tlist is left blank
  return cna;
}

bool Door::verify() {
  //SW::A should be a valid switch.
  if(switches[SW::A] < 0 || switches[SW::A] > 255) {
    return false;
  }
  if(args[0] < 0 || args[0] > 2) {
    return false;
  }
  return true;

}

Interface Door::behave(SwitchHandler* sh, Utility*) {
  //poll SW_Trig1
  bool sw = sh->read(switches[SW::A]);
  if(args[0] == 1) { //perma-open
    if(sw && !cooldown) {
      cooldown = true;
      locked = !locked;
    }
  }
  else if(args[0] == 0) {
    locked = !sw;
  }
  else {
    if(sw && !cooldown) {
      cooldown = true;
      locked = !locked;
    }
    else if(cooldown && !sw) {
      cooldown = false;
    }
  }
  if(locked) {
    status = Normal;
  }
  else {
    status = Inactive;
  }
  return Interface();
}
