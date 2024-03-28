#include "mutable/pushable.h"


Pushable::Pushable(int uid) : Object(uid) {}

Interface Pushable::interact(Object* p, Field* l, SwitchHandler*) {
  return Interface();
  //check what type the object is
  switch(p->Type()) {
    //can be pushed by entity and sliding
    case Object::Static:
    default:
      break;
    case Object::Sliding:
    case Object::Entity:
    case Object::Play:
      sf::Vector2f pmin{p->getPos()};
      sf::Vector2f pmax{pmin+p->getSize()-sf::Vector2f(1,1)};
      sf::Vector2f plmin{p->getLastPos()};
      sf::Vector2f plmax{plmin+p->getSize()-sf::Vector2f(1,1)};

      //just incase objects move, so the algorithm doesn't break
      sf::Vector2f omin{pos};
      sf::Vector2f omax{omin+size-sf::Vector2f(1,1)};
      sf::Vector2f olmin{lastPos};
      sf::Vector2f olmax{olmin+size-sf::Vector2f(1,1)};

      sf::Vector2f odelta = getDelta();
      sf::Vector2f pdelta = p->getDelta();
      //why is it only being pushed by '3'


      int tDist = pmax.y+1 - omin.y;
      int rDist = omax.x+1 - pmin.x;
      int bDist = omax.y+1 - pmin.y;
      int lDist = pmax.x+1 - omin.x;

      bool tMove = (tDist > 0 && tDist < rDist && tDist < bDist && tDist < lDist);
      bool rMove = (rDist > 0 && rDist < tDist && rDist < bDist && rDist < lDist);
      bool bMove = (bDist > 0 && bDist < rDist && bDist < tDist && bDist < lDist);
      bool lMove = (lDist > 0 && lDist < rDist && lDist < bDist && lDist < tDist);

      float lowestXDist = -1;
      float lowestYDist = -1;

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

      sf::Vector2f residSpeed;
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

        sf::Vector2f pCenter{(plmin.x + plmax.x) / 2, (plmin.y + plmax.y) / 2};
        sf::Vector2f oCenter{(olmin.x + olmax.x) / 2, (olmin.y + olmax.y) / 2};
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
      sf::Vector2f moveDistance;

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
