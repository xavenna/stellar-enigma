#include "callback.h"
#include <cmath>

// This file contains functions for handling interactions
// between player and entities/objects

//If I can figure it out, these eventually will be some kind of callback or function pointer
//That's why the file is named 'callback.cpp'
void nullBehave(Object& ob, Player& player, Level& level) {
  //do nothing

  //this is here b/c the compiler won't be happy otherwise
  ob.getXPos();
  player.getXPos();
  level.getTilesizeX();
}

void solidBehave(Object& ob, MapData* md) {
  sf::Vector2i pmin{md->player.getPos()};
  sf::Vector2i pmax{pmin+md->player.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{md->player.getLastPos()};
  sf::Vector2i plmax{plmin+md->player.getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{ob.getPos()};
  sf::Vector2i omax{omin+ob.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i olmin{ob.getLastPos()};
  sf::Vector2i olmax{olmin+ob.getSize()-sf::Vector2i(1,1)};
  //calculate delta-r
  sf::Vector2i deltaR = pmin - plmin;
  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  if(xInt && pmin.x < omin.x) {
    md->player.setXPos(ob.getXPos()-md->player.getSize().y);
  }
  if(yInt && pmin.y < omin.y) {
    md->player.setYPos(ob.getYPos()-md->player.getSize().x);
  }
  if(xInt && pmin.x > omin.x) {
    md->player.setXPos(ob.getXPos()+ob.getSize().x);
  }
  if(yInt && pmin.y > omin.y) {
    md->player.setYPos(ob.getYPos()+ob.getSize().y);
  }
}

void pushBehave(Object& ob, MapData* md) {
  //crate: attempts to move md->playerSpeed units in md->player facing direction
  
  sf::Vector2i pmin{md->player.getPos()};
  sf::Vector2i pmax{pmin+md->player.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{md->player.getLastPos()};
  sf::Vector2i plmax{plmin+md->player.getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{ob.getPos()};
  sf::Vector2i omax{omin+ob.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i olmin{ob.getLastPos()};
  sf::Vector2i olmax{olmin+ob.getSize()-sf::Vector2i(1,1)};
  //calculate delta-r
  sf::Vector2i deltaR = pmin - plmin;
  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  if(xInt && yInt) {
    return; //For now, diagonal
  }

  int px = 0;
  int py = 0;
  sf::Vector2i residSpeed;
  if(xInt && pmin.x < omin.x) {
    residSpeed.x = md->player.getXPos()+md->player.getSize().x-ob.getXPos();
    px = 1;
  }  //moving right
  if(yInt && pmin.y < omin.y) {
    residSpeed.y = md->player.getYPos()+md->player.getSize().y-ob.getYPos();
    py = 1;
  }
  if(xInt && pmin.x > omin.x) {
    residSpeed.x = ob.getXPos()+ob.getSize().x-md->player.getXPos();
    px = -1;
  }
  if(yInt && pmin.y > omin.y) {
    residSpeed.y = ob.getYPos()+ob.getSize().y-md->player.getYPos();
    py = -1;
  }
  sf::Vector2i moveDistance;
  moveDistance = md->levelSlot.validMove(ob.getPos(), ob.getSize(), residSpeed);
  /*
  if(px == 1) {
    moveDistance.x = md->levelSlot.validMove(ob.getPos(), ob.getSize(), residSpeed.x, Right);
  }
  else if(px == -1) {
    moveDistance.x = md->levelSlot.validMove(ob.getPos(), ob.getSize(), residSpeed.x, Left);
  }
  else {
    moveDistance.x = 0;
  }
  if(py == 1) {
    moveDistance.y = md->levelSlot.validMove(ob.getPos(), ob.getSize(), residSpeed.y, Down);
  }
  else if(py == -1) {
    moveDistance.y = md->levelSlot.validMove(ob.getPos(), ob.getSize(), residSpeed.y, Up);
  }
  else {
    moveDistance.y = 0;
  }
  */
  //moveDistance.y = md->levelSlot.validMove(ob.getPos(), ob.getSize(), residSpeed.y, md->player.getFacing());
  //move the object moveDistance Units in md->player.getFacing() direction
  if(xInt && pmin.x < omin.x) {
    ob.setXPos(ob.getXPos()+moveDistance.x);
    md->player.setXPos(md->player.getXPos()-(residSpeed.x-moveDistance.x));
  }
  if(yInt && pmin.y < omin.y) {
    ob.setYPos(ob.getYPos()+moveDistance.y);
    md->player.setYPos(md->player.getYPos()-(residSpeed.y-moveDistance.y));
  }
  if(xInt && pmin.x > omin.x) {
    ob.setXPos(ob.getXPos()-moveDistance.x);
    md->player.setXPos(md->player.getXPos()+(residSpeed.x-moveDistance.x));
  }
  if(yInt && pmin.y > omin.y) {
    ob.setYPos(ob.getYPos()-moveDistance.y);
    md->player.setYPos(md->player.getYPos()+(residSpeed.y-moveDistance.y));
  }
}
