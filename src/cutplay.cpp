#include "cutplay.h"

CutPlay::CutPlay(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, txt, a) {
  vars[0] = 0;
  args[1] = 30;
}

CutPlay::CutPlay() : Object() {
  vars[0] = 0;
  args[1] = 30;
}
CutPlay::CutPlay(Object ob) : Object(ob) {
  vars[0] = 0;
  args[1] = 30;
}

Interface CutPlay::interact(Player*, Field*, bool) {
  // play cutscene, nothing else (for now)
  if(!vars[0]) {
    if(args[0]) { 
      status = Destroy;
    }
    else {
      status = Normal;
    }

    vars[0] = args[1];
    active = false;
    return Interface(pos, "", text); 
  }
  else {
    return Interface(pos, "", "");
  }
}

Interface CutPlay::behave() {
  if(vars[0] > 0) {
    vars[0]--;
  }
  if(!vars[0]) {
    active = true;
  }
  return Interface(pos, "", "");
}
