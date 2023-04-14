#include "callback.h"
#include <cmath>

// This file contains functions for handling interactions
// between player and entities/objects

//If I can figure it out, these eventually will be some kind of callback or function pointer
//That's why the file is named 'callback.cpp'
//! Returns the coordinates the object would move to, in theory
sf::Vector2i nullBehave(Object* ob, MapData* md, bool dryRun) {
  //do nothing

  //this is here b/c the compiler won't be happy otherwise
  ob->getPos();
  dryRun = !dryRun;
  return md->player.getPos();
}

sf::Vector2i solidBehave(Object& ob, MapData* md, bool dryRun) {
  sf::Vector2i pmin{md->player.getPos()};
  sf::Vector2i pmax{pmin+md->player.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{md->player.getLastPos()};
  sf::Vector2i plmax{plmin+md->player.getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{ob.getPos()};
  sf::Vector2i omax{omin+ob.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i olmin{ob.getLastPos()};
  sf::Vector2i olmax{olmin+ob.getSize()-sf::Vector2i(1,1)};
  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  if(!dryRun) {
    if(xInt && pmin.x < omin.x) {
      md->player.setXPos(ob.getPos().x-md->player.getSize().y);
    }
    if(yInt && pmin.y < omin.y) {
      md->player.setYPos(ob.getPos().y-md->player.getSize().x);
    }
    if(xInt && pmin.x > omin.x) {
      md->player.setXPos(ob.getPos().x+ob.getSize().x);
    }
    if(yInt && pmin.y > omin.y) {
      md->player.setYPos(ob.getPos().y+ob.getSize().y);
    }
  }

  if(xInt && pmin.x < omin.x) {
    return sf::Vector2i(ob.getPos().x-md->player.getSize().y, md->player.getPos().y);
  }
  if(yInt && pmin.y < omin.y) {
    return sf::Vector2i(md->player.getPos().y, ob.getPos().y-md->player.getSize().x);
  }
  if(xInt && pmin.x > omin.x) {
    return sf::Vector2i(ob.getPos().x+ob.getSize().x, md->player.getPos().y);
  }
  if(yInt && pmin.y > omin.y) {
    return sf::Vector2i(md->player.getPos().x, ob.getPos().y+ob.getSize().y);
  }

  return (md->player.getPos());

}

void pushBehave(Object& ob, MapData* md) {
  //pushable object
  
  sf::Vector2i pmin{md->player.getPos()};
  sf::Vector2i pmax{pmin+md->player.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{md->player.getLastPos()};
  sf::Vector2i plmax{plmin+md->player.getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{ob.getPos()};
  sf::Vector2i omax{omin+ob.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i olmin{ob.getLastPos()};
  sf::Vector2i olmax{olmin+ob.getSize()-sf::Vector2i(1,1)};

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
    return; //For now, diagonal interactions are ignored
  }

  sf::Vector2i residSpeed;
  if(xInt && pmin.x < omin.x) {
    residSpeed.x = md->player.getPos().x+md->player.getSize().x-ob.getPos().x;
  }  //moving right
  if(yInt && pmin.y < omin.y) {
    residSpeed.y = md->player.getPos().y+md->player.getSize().y-ob.getPos().y;
  }
  if(xInt && pmin.x > omin.x) {
    residSpeed.x = md->player.getPos().x - (ob.getPos().x+ob.getSize().x);
  }
  if(yInt && pmin.y > omin.y) {
    residSpeed.y = md->player.getPos().y - (ob.getPos().y+ob.getSize().y);
  }
  sf::Vector2i moveDistance;
  moveDistance = md->levelSlot.validMove(ob.getPos(), ob.getSize(), residSpeed);
  ob.setPos(ob.getPos()+moveDistance);
  md->player.setPos(md->player.getPos() -(residSpeed - moveDistance));
}


void hurtBehave(Object& ob, MapData* md) {
  sf::Vector2i pmin{md->player.getPos()};
  sf::Vector2i pmax{pmin+md->player.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{md->player.getLastPos()};
  sf::Vector2i plmax{plmin+md->player.getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{ob.getPos()};
  sf::Vector2i omax{omin+ob.getSize()-sf::Vector2i(1,1)};
  sf::Vector2i olmin{ob.getLastPos()};
  sf::Vector2i olmax{olmin+ob.getSize()-sf::Vector2i(1,1)};
  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  bool hurt = false;
  if(xInt && pmin.x < omin.x) {
    md->player.setXPos(ob.getPos().x-md->player.getSize().y);
    hurt = true;
  }
  if(yInt && pmin.y < omin.y) {
    md->player.setYPos(ob.getPos().y-md->player.getSize().x);
    hurt = true;
  }
  if(xInt && pmin.x > omin.x) {
    md->player.setXPos(ob.getPos().x+ob.getSize().x);
    hurt = true;
  }
  if(yInt && pmin.y > omin.y) {
    md->player.setYPos(ob.getPos().y+ob.getSize().y);
    hurt = true;
  }
  if(hurt) {
    if(md->player.getCooldown() == 0) {
      md->player.modifyHealth(-1);
      md->player.damaged = true;
    }
  }
}
