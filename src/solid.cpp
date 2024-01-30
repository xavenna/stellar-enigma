#include "solid.h"

Solid::Solid(int uid) : Object(uid) {}


bool Solid::verify() {
  //doesn't use any args or switches, those can be ignored
  //if texture-id is invalid, stone is used.
  //Thus, no invalid configs exist
  return true;
}

Interface Solid::interact(Object* p, Field*, SwitchHandler*) {
  interacting = false;
  //push back slidings and entities, ignore solids
  switch(p->Type()) {
    case Object::Static:
    default:
      //solids can't push other solids
      break;
    case Object::Entity:
    case Object::Sliding:
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

      int tDist = pmax.y - omin.y;
      int rDist = omax.x - pmin.x;
      int bDist = omax.y - pmin.y;
      int lDist = pmax.x - omin.x;

      sf::Vector2i pDelta{pmin.x - plmin.x, pmin.y-plmin.y};

      /*
      if(p->Name() == "pushable") {
        std::cerr << getLastPos().x << ',' << p->getLastPos().y << '\n';
        std::cerr << getPos().x << ',' << p->getPos().y << "\n\n";
      }
      */

      if(tDist > 0 && tDist < rDist && tDist < bDist && tDist < lDist) {
        interacting = true;
        p->setYPos(omin.y - p->getSize().y);
      }
      else if(rDist > 0 && rDist < tDist && rDist < bDist && rDist < lDist) {
        interacting = true;
        p->setXPos(omax.x+1);
      }
      else if(bDist > 0 && bDist < rDist && bDist < tDist && bDist < lDist) {
        interacting = true;
        p->setYPos(omax.y+1);
      }
      else if(lDist > 0 && lDist < rDist && lDist < bDist && lDist < tDist) {
        interacting = true;
        p->setXPos(omin.x - p->getSize().x);
      }
      else if(lDist > 0 && lDist == tDist && lDist < rDist && lDist < bDist) {
        //top left
        if(pDelta.x < pDelta.y) {
          //push up
          p->setYPos(omin.y - p->getSize().y);
        }
        else if(pDelta.x > pDelta.y) {
          //push left
          p->setXPos(omin.x - p->getSize().x);
        }
        else {
          //push diagonally
          p->setXPos(omin.x - p->getSize().x);
          p->setYPos(omin.y - p->getSize().y);
        }
        interacting = true;
      }
      else if(lDist > 0 && lDist == bDist && lDist < rDist && lDist < tDist) {
        //bottom left
        if(pDelta.x > -pDelta.y) {
          //push down
          p->setYPos(omax.y + 1);
        }
        else if(pDelta.x < -pDelta.y) {
          //push left
          p->setXPos(omin.x - p->getSize().x);
        }
        else {
          //push diagonally
          p->setXPos(omin.x - p->getSize().x);
          p->setYPos(omax.y + 1);
        }
        interacting = true;
      }
      else if(rDist > 0 && rDist == tDist && rDist < lDist && rDist < bDist) {
        //top right
        if(-pDelta.x > pDelta.y) {
          //push up
          p->setYPos(omin.y - p->getSize().y);
        }
        else if(-pDelta.x < pDelta.y) {
          //push right
          p->setXPos(omax.x + 1);
        }
        else {
          //push diagonally
          p->setXPos(omax.x + 1);
          p->setYPos(omin.y - p->getSize().y);
        }
        interacting = true;
      }
      else if(rDist > 0 && rDist == bDist && rDist < lDist && rDist < tDist) {
        //bottom right
        if(pDelta.x > pDelta.y) {
          //push down
          p->setYPos(omax.y + 1);
        }
        else if(pDelta.x < pDelta.y) {
          //push right
          p->setXPos(omax.x + 1);
        }
        else {
          //push diagonally
          p->setXPos(omax.x + 1);
          p->setYPos(omax.y + 1);
        }
        interacting = true;
      }

      /*
      bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
      bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

      bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
      bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

      //x, y interactions
      bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
      bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

      if(xInt && pmin.x < omin.x) {
        p->setXPos(pos.x-p->getSize().y);
        interacting = true;
      }
      if(yInt && pmin.y < omin.y) {
        p->setYPos(pos.y-p->getSize().x);
        interacting = true;
      }
      if(xInt && pmin.x > omin.x) {
        p->setXPos(pos.x+size.x);
        interacting = true;
      }
      if(yInt && pmin.y > omin.y) {
        p->setYPos(pos.y+size.y);
        interacting = true;
      }
      */
  }

  return Interface();
  
}

Interface Solid::interact(Player* p, Field*, SwitchHandler*) {
  interacting = false;

  sf::Vector2i pmin{p->getPos()};
  sf::Vector2i pmax{pmin+p->getSize()-sf::Vector2i(1,1)};
  sf::Vector2i plmin{p->getLastPos()};
  sf::Vector2i plmax{plmin+p->getSize()-sf::Vector2i(1,1)};

  //just incase objects move, so the algorithm doesn't break
  sf::Vector2i omin{pos};
  sf::Vector2i omax{omin+size-sf::Vector2i(1,1)};
  sf::Vector2i olmin{lastPos};
  sf::Vector2i olmax{olmin+size-sf::Vector2i(1,1)};

  sf::Vector2i pDelta = pmin - plmin;

  int tDist = pmax.y - omin.y;
  int rDist = omax.x - pmin.x;
  int bDist = omax.y - pmin.y;
  int lDist = pmax.x - omin.x;

  if(tDist > 0 && tDist < rDist && tDist < bDist && tDist < lDist) {
    interacting = true;
    p->setYPos(omin.y - p->getSize().y);
  }
  else if(rDist > 0 && rDist < tDist && rDist < bDist && rDist < lDist) {
    interacting = true;
    p->setXPos(omax.x + 1);
  }
  else if(bDist > 0 && bDist < rDist && bDist < tDist && bDist < lDist) {
    interacting = true;
    p->setYPos(omax.y + 1);
  }
  else if(lDist > 0 && lDist < rDist && lDist < bDist && lDist < tDist) {
    interacting = true;
    p->setXPos(omin.x - p->getSize().x);
  }
  else if(lDist > 0 && lDist == tDist && lDist < rDist && lDist < bDist) {
    //top left
    if(pDelta.x < pDelta.y) {
      //push up
      p->setYPos(omin.y - p->getSize().y);
    }
    else if(pDelta.x > pDelta.y) {
      //push left
      p->setXPos(omin.x - p->getSize().x);
    }
    else {
      //push diagonally
      p->setXPos(omin.x - p->getSize().x);
      p->setYPos(omin.y - p->getSize().y);
    }
    interacting = true;
  }
  else if(lDist > 0 && lDist == bDist && lDist < rDist && lDist < tDist) {
    //bottom left
    if(pDelta.x > -pDelta.y) {
      //push down
      p->setYPos(omax.y + 1);
    }
    else if(pDelta.x < -pDelta.y) {
      //push left
      p->setXPos(omin.x - p->getSize().x);
    }
    else {
      //push diagonally
      p->setXPos(omin.x - p->getSize().x);
      p->setYPos(omax.y + 1);
    }
    interacting = true;
  }
  else if(rDist > 0 && rDist == tDist && rDist < lDist && rDist < bDist) {
    //top right
    if(-pDelta.x > pDelta.y) {
      //push up
      p->setYPos(omin.y - p->getSize().y);
    }
    else if(-pDelta.x < pDelta.y) {
      //push right
      p->setXPos(omax.x + 1);
    }
    else {
      //push diagonally
      p->setXPos(omax.x + 1);
      p->setYPos(omin.y - p->getSize().y);
    }
    interacting = true;
  }
  else if(rDist > 0 && rDist == bDist && rDist < lDist && rDist < tDist) {
    //bottom right
    if(pDelta.x > pDelta.y) {
      //push down
      p->setYPos(omax.y + 1);
    }
    else if(pDelta.x < pDelta.y) {
      //push right
      p->setXPos(omax.x + 1);
    }
    else {
      //push diagonally
      p->setXPos(omax.x + 1);
      p->setYPos(omax.y + 1);
    }
    interacting = true;
  }
  //check for diagonal movements

  /*
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
    p->setXPos(pos.x-p->getSize().y);
    interacting = true;
  }
  if(yInt && pmin.y < omin.y) {
    p->setYPos(pos.y-p->getSize().x);
    interacting = true;
  }
  if(xInt && pmin.x > omin.x) {
    p->setXPos(pos.x+size.x);
    interacting = true;
  }
  if(yInt && pmin.y > omin.y) {
    p->setYPos(pos.y+size.y);
    interacting = true;
  }
  */

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


Interface Solid::behave(SwitchHandler*, Utility*) {
  //no special behaviors
  return Interface();
}
