#include "pushable.h"


Pushable::Pushable(int uid) : Object(uid) {}
Pushable::Pushable(Object ob) : Object(ob) {}


Interface Pushable::interact(Player* p, Field* l, SwitchHandler*) {
  //if the crate moves into the player, switch who moves
  sf::Vector2i pmin{p->getPos()};
  sf::Vector2i pmax{pmin+p->getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{p->getLastPos()};
  sf::Vector2i plmax{plmin+p->getSize()-sf::Vector2i(1,1)};
  sf::Vector2i pdelta = pmin - plmin;

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{pos};
  sf::Vector2i omax{omin+size-sf::Vector2i(1,1)};
  sf::Vector2i olmin{lastPos};
  sf::Vector2i olmax{olmin+size-sf::Vector2i(1,1)};
  sf::Vector2i odelta = omin - olmin;

  bool pInit = true;  //does the player initiate action?
  if((odelta.x || odelta.y) && !(pdelta.x && pdelta.y)) {
    //object initiates
    pInit = false;
  }

  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  /*
  if(xInt && yInt) {
    return Interface(pos, "", ""); //For now, diagonal interactions are ignored
    //so, this means the player can clip into pushables by moving diagonally
    //which is bad, fix this
  }
  */

  if(pInit) {
    sf::Vector2i residSpeed;
    if(xInt && pmin.x < omin.x) {
      residSpeed.x = p->getPos().x+p->getSize().x-pos.x;
    }
    else if(xInt && pmin.x > omin.x) {
      residSpeed.x = p->getPos().x - (pos.x+size.x);
    }
    if(yInt && pmin.y < omin.y) {
      residSpeed.y = p->getPos().y+p->getSize().y-pos.y;
    }
    else if(yInt && pmin.y > omin.y) {
      residSpeed.y = p->getPos().y - (pos.y+size.y);
    }
    sf::Vector2i moveDistance;

    moveDistance = l->validMove(pos, size, residSpeed);

    pos = pos+moveDistance;
    p->setPos(p->getPos() -(residSpeed - moveDistance));
    return Interface();
  }
  else {
    sf::Vector2i residSpeed;
    if(xInt && omin.x < pmin.x) {
      //residSpeed.x = p->getPos().x+p->getSize().x-pos.x;
      residSpeed.x = pos.x + size.x - p->getPos().x;
    }  //moving right
    if(yInt && omin.y < pmin.y) {
      //residSpeed.y = p->getPos().y+p->getSize().y-pos.y;
      residSpeed.y = pos.y + size.y - p->getPos().y;
    }
    if(xInt && omin.x > pmin.x) {
      //residSpeed.x = p->getPos().x - (pos.x+size.x);
      residSpeed.x = pos.x - (p->getPos().x + p->getSize().x);
    }
    if(yInt && omin.y > pmin.y) {
      //residSpeed.y = p->getPos().y - (pos.y+size.y);
      residSpeed.y = pos.y - (p->getPos().y + p->getSize().y);
    }
    sf::Vector2i moveDistance;

    moveDistance = l->validMove(p->getPos(), p->getSize(), residSpeed);

    //pos = pos+moveDistance;
    p->setPos(p->getPos() + moveDistance);
    //p->setPos(p->getPos() -(residSpeed - moveDistance));
    pos = pos - (residSpeed - moveDistance);
    return Interface();
  }
}

Interface Pushable::interact(Object* p, Field* l, SwitchHandler*) {
  //check what type the object is
  switch(p->Type()) {
    //can be pushed by entity and sliding
    case Object::Static:
      break;
    case Object::Sliding:
    case Object::Entity:
      sf::Vector2i pmin{p->getPos()};
      sf::Vector2i pmax{pmin+p->getSize()-sf::Vector2i(1,1)};
      sf::Vector2i plmin{p->getLastPos()};
      sf::Vector2i plmax{plmin+p->getSize()-sf::Vector2i(1,1)};

      //just incase objects move, so the algorithm doesn't break
      sf::Vector2i omin{pos};
      sf::Vector2i omax{omin+size-sf::Vector2i(1,1)};
      sf::Vector2i olmin{lastPos};
      sf::Vector2i olmax{olmin+size-sf::Vector2i(1,1)};

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
        return Interface(); //For now, diagonal interactions are ignored
      }

      sf::Vector2i residSpeed;
      if(xInt && pmin.x < omin.x) {
        residSpeed.x = p->getPos().x+p->getSize().x-pos.x;
      }  //moving right
      if(yInt && pmin.y < omin.y) {
        residSpeed.y = p->getPos().y+p->getSize().y-pos.y;
      }
      if(xInt && pmin.x > omin.x) {
        residSpeed.x = p->getPos().x - (pos.x+size.x);
      }
      if(yInt && pmin.y > omin.y) {
        residSpeed.y = p->getPos().y - (pos.y+size.y);
      }
      sf::Vector2i moveDistance;

      moveDistance = l->validMove(pos, size, residSpeed);
      // I need to find a way to get this to work, as Level* or MapData* can't be passed in

      pos = pos+moveDistance;
      p->setPos(p->getPos() -(residSpeed - moveDistance));
      break;
  }
  return Interface();
}


CacheNodeAttributes Pushable::draw(const TextureCache* cache) {
  // draw a blank tile with no transforms
  CacheNodeAttributes cna;
  switch(args[0]) {
  default:
    cna.srcImg = cache->reverseHash("crate");
  //others can go here for more freedom
  }
  //no transforms, so tlist is left blank
  return cna;
}
