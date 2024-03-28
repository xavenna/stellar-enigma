#include "mutable/solid.h"

Solid::Solid(int uid) : Object(uid) {}


bool Solid::verify() {
  //doesn't use any args or switches, those can be ignored
  //if texture-id is invalid, stone is used.
  //Thus, no invalid configs exist
  return true;
}

Interface Solid::interact(Object* p, Field* f, SwitchHandler*) {
  return Interface();
  interacting = false;
  //push back slidings and entities, ignore solids
  switch(p->Type()) {
    case Object::Static:
    default:
      //solids can't push other solids
      break;
    case Object::Entity:
    case Object::Sliding:
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
      //determine interaction for each direction:
      //was player intersecting with object on the x-axis?

      float tDist = pmax.y - omin.y;
      float rDist = omax.x - pmin.x;
      float bDist = omax.y - pmin.y;
      float lDist = pmax.x - omin.x;

      sf::Vector2f pDelta{pmin.x - plmin.x, pmin.y-plmin.y};

      sf::Vector2f pMove(pmin);

      if(tDist > 0 && tDist < rDist && tDist < bDist && tDist < lDist) {
        interacting = true;
        pMove.y = (omin.y - p->getSize().y);
      }
      else if(rDist > 0 && rDist < tDist && rDist < bDist && rDist < lDist) {
        interacting = true;
        pMove.x = (omax.x+1);
      }
      else if(bDist > 0 && bDist < rDist && bDist < tDist && bDist < lDist) {
        interacting = true;
        pMove.y = (omax.y+1);
      }
      else if(lDist > 0 && lDist < rDist && lDist < bDist && lDist < tDist) {
        interacting = true;
        pMove.x = (omin.x - p->getSize().x);
      }
      else if(lDist > 0 && lDist == tDist && lDist < rDist && lDist < bDist) {
        //top left
        if(pDelta.x < pDelta.y) {
          //push up
          pMove.y = (omin.y - p->getSize().y);
        }
        else if(pDelta.x > pDelta.y) {
          //push left
          pMove.x = (omin.x - p->getSize().x);
        }
        else {
          //push diagonally
          pMove.x = (omin.x - p->getSize().x);
          pMove.y = (omin.y - p->getSize().y);
        }
        interacting = true;
      }
      else if(lDist > 0 && lDist == bDist && lDist < rDist && lDist < tDist) {
        //bottom left
        if(pDelta.x > -pDelta.y) {
          //push down
          pMove.y = (omax.y + 1);
        }
        else if(pDelta.x < -pDelta.y) {
          //push left
          pMove.x = (omin.x - p->getSize().x);
        }
        else {
          //push diagonally
          pMove.x = (omin.x - p->getSize().x);
          pMove.y = (omax.y + 1);
        }
        interacting = true;
      }
      else if(rDist > 0 && rDist == tDist && rDist < lDist && rDist < bDist) {
        //top right
        if(-pDelta.x > pDelta.y) {
          //push up
          pMove.y = (omin.y - p->getSize().y);
        }
        else if(-pDelta.x < pDelta.y) {
          //push right
          pMove.x = (omax.x + 1);
        }
        else {
          //push diagonally
          pMove.x = (omax.x + 1);
          pMove.y = (omin.y - p->getSize().y);
        }
        interacting = true;
      }
      else if(rDist > 0 && rDist == bDist && rDist < lDist && rDist < tDist) {
        //bottom right
        if(pDelta.x > pDelta.y) {
          //push down
          pMove.y = (omax.y + 1);
        }
        else if(pDelta.x < pDelta.y) {
          //push right
          pMove.x = (omax.x + 1);
        }
        else {
          //push diagonally
          pMove.x = (omax.x + 1);
          pMove.y = (omax.y + 1);
        }
        interacting = true;
      }
      pMove = pMove - p->getPos();
      sf::Vector2f resid = f->validMove(p->getPos(), p->getSize(), pMove);
      p->setPos(p->getPos()+resid);
  }

  return Interface();
  
}



CacheNodeAttributes Solid::draw(const TextureCache* cache) {
  // draw a solid object with no transforms
  // use obj_arg[0] to decide which texture to draw
  CacheNodeAttributes cna;
  switch(texture_id) {
  case 7:
    cna.srcImg = cache->reverseHash("yim");
    break;
  default:
    cna.srcImg = cache->reverseHash("stone");
  //other textures can be placed here for more design freedom
  }
  //no transforms, so tlist is left blank
  return cna;
}
