#include "pushable.h"


Pushable::Pushable(int uid) : Object(uid) {}


Interface Pushable::interact(Player* p, Field* l, SwitchHandler*) {
  //if the crate moves into the player, switch who moves
  sf::Vector2i pmin{p->getPos()};
  sf::Vector2i pmax{pmin+p->getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{p->getLastPos()};
  sf::Vector2i plmax{plmin+p->getSize()-sf::Vector2i(1,1)};
  sf::Vector2i pdelta = p->getDelta();

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{pos};
  sf::Vector2i omax{omin+size-sf::Vector2i(1,1)};
  sf::Vector2i olmin{lastPos};
  sf::Vector2i olmax{olmin+size-sf::Vector2i(1,1)};
  sf::Vector2i odelta = getDelta();

  bool pInit = true;  //does the player initiate action?
  if((odelta.x || odelta.y) && !(pdelta.x || pdelta.y)) {
    //object initiates
    pInit = false;
  }


  int tDist = pmax.y+1 - omin.y;
  int rDist = omax.x+1 - pmin.x;
  int bDist = omax.y+1 - pmin.y;
  int lDist = pmax.x+1 - omin.x;

  //if one of these distances is negative or zero, no collision occurs on that side.

  int lowestXDist = -1;
  int lowestYDist = -1;

  if(rDist > 0 && lDist > 0) {
    lowestXDist = (rDist < lDist ? rDist : lDist);
  }
  else if(rDist > 0 && lDist <= 0) {
    lowestXDist = rDist;
  }
  else if(rDist <= 0 && lDist > 0) {
    lowestXDist = lDist;
  }
  else {
    lowestXDist = -1;
  }
  if(tDist > 0 && bDist > 0) {
    lowestYDist = (tDist < bDist ? tDist : bDist);
  }
  else if(tDist > 0 && bDist <= 0) {
    lowestYDist = tDist;
  }
  else if(tDist <= 0 && bDist > 0) {
    lowestYDist = bDist;
  }
  else {
    lowestYDist = -1;
  }

  bool tMove = (tDist > 0 && tDist < rDist && tDist < bDist && tDist < lDist);
  bool rMove = (rDist > 0 && rDist < tDist && rDist < bDist && rDist < lDist);
  bool bMove = (bDist > 0 && bDist < rDist && bDist < tDist && bDist < lDist);
  bool lMove = (lDist > 0 && lDist < rDist && lDist < bDist && lDist < tDist);




  //determine interaction for each direction:
  //was player intersecting with object on the x-axis?
  bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
  bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

  bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
  bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

  //x, y interactions
  bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
  bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

  if(pInit) {
    sf::Vector2i residSpeed;
    if(lowestXDist != lowestYDist) { //this won't work on non-square objects
      if(lMove) {
        residSpeed.x = p->getPos().x+p->getSize().x-pos.x;
      }
      else if(rMove) {
        residSpeed.x = p->getPos().x - (pos.x+size.x);
      }
      if(tMove) {
        residSpeed.y = p->getPos().y+p->getSize().y-pos.y;
      }
      else if(bMove) {
        residSpeed.y = p->getPos().y - (pos.y+size.y);
      }
    }
    else {
      sf::Vector2f pCenter{static_cast<float>(plmin.x + plmax.x) / 2, static_cast<float>(plmin.y + plmax.y) / 2};
      sf::Vector2f oCenter{static_cast<float>(olmin.x + olmax.x) / 2, static_cast<float>(olmin.y + olmax.y) / 2};
      sf::Vector2f pointDir = oCenter - pCenter; 
      //only keep the components of pdelta with the same speed as pCenter

      //the magnitude should be based on how far the player overlaps
      if(sign(pointDir.x) == sign(pdelta.x)) {
        residSpeed.x = pdelta.x;
      }
      if(sign(pointDir.y) == sign(pdelta.y)) {
        residSpeed.y = pdelta.y;
      }
    }
    sf::Vector2i moveDistance;

    moveDistance = l->validMove(pos, size, residSpeed);

    pos = pos+moveDistance;
    p->setPos(p->getPos() -(residSpeed - moveDistance));
    return Interface();
  }
  else {
    sf::Vector2i residSpeed;
    if(lowestXDist != lowestYDist) {
      if(rMove) {
        residSpeed.x = pos.x + size.x - p->getPos().x;
      }  //moving right
      else if(lMove) {
        residSpeed.x = pos.x - (p->getPos().x + p->getSize().x);
      }
      if(bMove) {
        residSpeed.y = pos.y + size.y - p->getPos().y;
      }
      else if(tMove) {
        residSpeed.y = pos.y - (p->getPos().y + p->getSize().y);
      }
    }
    else {
      sf::Vector2f pCenter{static_cast<float>(plmin.x + plmax.x) / 2, static_cast<float>(plmin.y + plmax.y) / 2};
      sf::Vector2f oCenter{static_cast<float>(olmin.x + olmax.x) / 2, static_cast<float>(olmin.y + olmax.y) / 2};
      sf::Vector2f pointDir = oCenter - pCenter; 
      //only keep the components of pdelta with the same speed as pCenter

      //the magnitude should be based on how far the player overlaps
      if(sign(pointDir.x) == sign(pdelta.x)) {
        residSpeed.x = pdelta.x;
      }
      if(sign(pointDir.y) == sign(pdelta.y)) {
        residSpeed.y = pdelta.y;
      }
    }

    sf::Vector2i moveDistance;

    moveDistance = l->validMove(p->getPos(), p->getSize(), residSpeed);

    p->setPos(p->getPos() + moveDistance);
    pos = pos - (residSpeed - moveDistance);
    return Interface();
  }
}

Interface Pushable::interact(Object* p, Field* l, SwitchHandler*) {
  //check what type the object is
  switch(p->Type()) {
    //can be pushed by entity and sliding
    case Object::Static:
    default:
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

      sf::Vector2i odelta = getDelta();
      sf::Vector2i pdelta = p->getDelta();
      //why is it only being pushed by '3'


      int tDist = pmax.y+1 - omin.y;
      int rDist = omax.x+1 - pmin.x;
      int bDist = omax.y+1 - pmin.y;
      int lDist = pmax.x+1 - omin.x;

      bool tMove = (tDist > 0 && tDist < rDist && tDist < bDist && tDist < lDist);
      bool rMove = (rDist > 0 && rDist < tDist && rDist < bDist && rDist < lDist);
      bool bMove = (bDist > 0 && bDist < rDist && bDist < tDist && bDist < lDist);
      bool lMove = (lDist > 0 && lDist < rDist && lDist < bDist && lDist < tDist);

      int lowestXDist = -1;
      int lowestYDist = -1;

      if(rDist > 0 && lDist > 0) {
        lowestXDist = (rDist < lDist ? rDist : lDist);
      }
      else if(rDist > 0 && lDist <= 0) {
        lowestXDist = rDist;
      }
      else if(rDist <= 0 && lDist > 0) {
        lowestXDist = lDist;
      }
      else {
        lowestXDist = -1;
      }
      if(tDist > 0 && bDist > 0) {
        lowestYDist = (tDist < bDist ? tDist : bDist);
      }
      else if(tDist > 0 && bDist <= 0) {
        lowestYDist = tDist;
      }
      else if(tDist <= 0 && bDist > 0) {
        lowestYDist = bDist;
      }
      else {
        lowestYDist = -1;
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


      sf::Vector2i residSpeed;
      if(lowestXDist != lowestYDist) {
        if(lMove) {
          residSpeed.x = pmin.x+p->getSize().x-pos.x;
        }
        else if(rMove) {
          residSpeed.x = pmin.x - (pos.x+size.x);
        }
        if(tMove) {
          residSpeed.y = pmin.y+p->getSize().y-pos.y;
        }
        else if(bMove) {
          residSpeed.y = pmin.y - (pos.y+size.y);
        }
      }
      else {
        //p is moving into o diagonally
        residSpeed = pdelta;

        sf::Vector2f pCenter{static_cast<float>(plmin.x + plmax.x) / 2, static_cast<float>(plmin.y + plmax.y) / 2};
        sf::Vector2f oCenter{static_cast<float>(olmin.x + olmax.x) / 2, static_cast<float>(olmin.y + olmax.y) / 2};
        sf::Vector2f pointDir = oCenter - pCenter; 
        //only keep the components of pdelta with the same speed as pCenter

        //the magnitude should be based on how far the player overlaps
        if(sign(pointDir.x) == sign(pdelta.x)) {
          residSpeed.x = pdelta.x;
        }
        if(sign(pointDir.y) == sign(pdelta.y)) {
          residSpeed.y = pdelta.y;
        }
      }
      sf::Vector2i moveDistance;

      moveDistance = l->validMove(pos, size, residSpeed);

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
