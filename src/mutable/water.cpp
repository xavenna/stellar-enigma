#include "mutable/water.h"



CacheNodeAttributes Water::draw() {

  CacheNodeAttributes cna;
  cna.srcImg = "water";

    Transform t;
    t.type = Transform::Set_Width;
    t.args[0] = static_cast<int>(16);
    cna.tList.push_back(t);  //set width to one tile

    Transform t2;
    t2.type = Transform::Slide_X;
    t2.args[0] = 2*(int(animTimer/10)%8);
    cna.tList.push_back(t2);

    t.type = Transform::Rotate;
    t.args[0] = 0;
    cna.tList.push_back(t);

    t.type = Transform::Tile_Grow;
    t.args[0] = texSize.x;
    t.args[1] = texSize.y;
    cna.tList.push_back(t);

  return cna;
}
Interface Water::behave(SwitchHandler*, Utility* u) {
  animTimer = u->getFrame();
  if(!setup) {
    texSize = static_cast<sf::Vector2i>(getESize());
    scaleFactor = sf::Vector2f(1,1);
    setup = true;
  }
  return Interface();
}

bool Water::verify() {
  return true;
}

Water::Water(int uid) : Object{uid} {
  //okay, so here the obj needs to set it's scale to 1:1, 

}
