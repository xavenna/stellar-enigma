#include "spawner.h"

Spawner::Spawner() : Object() {
  vars[0] = 0;
}
Spawner::Spawner(Object ob) : Object(ob) {
  vars[0] = 0;
}
Spawner::Spawner(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, txt, a) {
  vars[0] = 0;
}


Interface Spawner::behave(Player*, Field*, bool) {
  //args[0] is a binary concatenation of delay between spawns and max number of objs to
     //spawn (0 means infinite)
  //args[1] is object type
  //args[2] is bool: spawn exactly at pos? (false means spawn anywhere within size
  //args[3] is a binary concatenation of target object size
  //args[4] is a binary concatenation of spawned args[0],args[1]
  //args[5] is a binary concatenation of spawned args[2],args[3]
  //args[6] is a binary concatenation of spawned args[4],args[5]
  //args[7] is a binary concatenation of spawned args[6],args[7]
  //text is text arg of spawned objects

  //vars[0] is remaining cooldown
  //vars[1] is remaining number of objs to spawn
  std::vector<Object> obs;
  if(vars[0] == 0) {
    //calculate args
    std::array<int, 8> a;
    sf::Vector2i p;
    sf::Vector2i s = sf::Vector2i(binDecat(args[2]).x, binDecat(args[2]).y);
    a[0] = binDecat(args[4]).x;
    a[1] = binDecat(args[4]).y;
    a[2] = binDecat(args[5]).x;
    a[3] = binDecat(args[5]).y;
    a[4] = binDecat(args[6]).x;
    a[5] = binDecat(args[6]).y;
    a[6] = binDecat(args[7]).x;
    a[7] = binDecat(args[7]).y;
    if(args[2]) {
      p = pos;
    }
    else {
      //set position to random position within box
      p = pos;
    }
    //spawn object
    obs.push_back(Object(p.x, p.y, s.x, s.y, args[1], value, solid, text, a));
    vars[0] = binDecat(args[0]).x;
    if(binDecat(args[0]).y) {
      //if number of spawns is limited
      if(vars[1]) {
        vars[1]--;
      }
      else {
        status = Object::Destroy;
      }
    }

    
  }
  else {
    vars[0]--;
  }
  return Interface(pos, "", "");
}

