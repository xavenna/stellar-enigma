#include "callback.h"

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

//so, diagonal movement breaks this
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

  if(xInt && pmin.x <= omin.x) {
    md->player.setXPos(ob.getXPos()-md->player.getSize().y);
  }
  if(yInt && pmin.y <= omin.y) {
    md->player.setYPos(ob.getYPos()-md->player.getSize().x);
  }
  if(xInt && pmin.x >= omin.x) {
    md->player.setXPos(ob.getXPos()+ob.getSize().x);
  }
  if(yInt && pmin.y >= omin.y) {
    md->player.setYPos(ob.getYPos()+ob.getSize().y);
  }
}

//so, diagonal movement breaks this
void pushBehave(Object& ob, MapData* md) {
  //crate: attempts to move md->playerSpeed units in md->player facing direction
  
  int residSpeed;
  switch(md->player.getFacing()) {
  case Right:
    residSpeed = md->player.getXPos()+md->player.getSize().x-ob.getXPos();
    break;
  case Down:
    residSpeed = md->player.getYPos()+md->player.getSize().y-ob.getYPos();
    break;
  case Left:
    residSpeed = ob.getXPos()+ob.getSize().x-md->player.getXPos();
    break;
  case Up:
    residSpeed = ob.getYPos()+ob.getSize().y-md->player.getYPos();
    break;
  }
  int moveDistance = md->levelSlot.validMove(ob.getPos(), ob.getSize(), residSpeed, md->player.getFacing());
  //move the object moveDistance Units in md->player.getFacing() direction
  switch(md->player.getFacing()) {
  case Right:
    ob.setXPos(ob.getXPos()+moveDistance);
    md->player.setXPos(md->player.getXPos()-(residSpeed-moveDistance));
    break;
  case Down:
    ob.setYPos(ob.getYPos()+moveDistance);
    md->player.setYPos(md->player.getYPos()-(residSpeed-moveDistance));
    break;
  case Left:
    ob.setXPos(ob.getXPos()-moveDistance);
    md->player.setXPos(md->player.getXPos()+(residSpeed-moveDistance));
    break;
  case Up:
    ob.setYPos(ob.getYPos()-moveDistance);
    md->player.setYPos(md->player.getYPos()+(residSpeed-moveDistance));
    break;
  }
}
