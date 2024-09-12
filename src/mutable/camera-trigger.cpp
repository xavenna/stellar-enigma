#include "mutable/camera-trigger.h"


CameraTrig::CameraTrig(int uid) : Object(uid) {
  cooldown = 0;
}

Interface CameraTrig::interact(Object* o, Field*, SwitchHandler*) {
  // trigger a camera change
  Interface inter;
  if(o->Type() == Object::Play) {
    if(!cooldown) {
      if(args[0]) { 
        status = Destroy;
      }
      else {
        status = Inactive;
      }
      
      cooldown = args[1];
      //trigger a camera change
      inter.selectConfig(text);
    }
  }
  return inter;
}

Interface CameraTrig::behave(SwitchHandler*, Utility*) {
  if(cooldown > 0) {
    cooldown--;
  }
  if(!cooldown) {
    status = Normal;
  }
  return Interface();
}
