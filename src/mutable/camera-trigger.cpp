#include "mutable/camera-trigger.h"


CameraTrig::CameraTrig(int uid) : Object(uid) {
  vars[0] = 0;
}

Interface CameraTrig::interact(Object* o, Field*, SwitchHandler*) {
  // play cutscene, nothing else (for now)
  Interface inter;
  if(o->Type() == Object::Play) {
    if(!vars[0]) {
      if(args[0]) { 
        status = Destroy;
      }
      else {
        status = Inactive;
      }
      
      vars[0] = args[1];
      //trigger a camera change
    }
  }
  return inter;
}

Interface CameraTrig::behave(SwitchHandler*, Utility*) {
  if(vars[0] > 0) {
    vars[0]--;
  }
  if(!vars[0]) {
    status = Normal;
  }
  return Interface();
}
