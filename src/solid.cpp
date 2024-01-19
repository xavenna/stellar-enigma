#include "solid.h"

Solid::Solid(int uid) : Object(uid) {}


Interface Solid::interact(Object* p, Field*, SwitchHandler*) {
  //push back slidings and entities, ignore solids
  switch(p->Type()) {
    case Object::Static:
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
      bool xAfter = !(omax.x < pmin.x || omin.x > pmax.x);
      bool xBefore = !(olmax.x < plmin.x || olmin.x > plmax.x);

      bool yAfter = !(omax.y < pmin.y || omin.y > pmax.y);
      bool yBefore = !(olmax.y < plmin.y || olmin.y > plmax.y);

      //x, y interactions
      bool xInt = xAfter && !xBefore && ((yAfter && yBefore) || (!yBefore && yAfter));
      bool yInt = yAfter && !yBefore && ((xAfter && xBefore) || (!xBefore && xAfter));

      if(xInt && pmin.x < omin.x) {
        p->setXPos(pos.x-p->getSize().y);
      }
      if(yInt && pmin.y < omin.y) {
        p->setYPos(pos.y-p->getSize().x);
      }
      if(xInt && pmin.x > omin.x) {
        p->setXPos(pos.x+size.x);
      }
      if(yInt && pmin.y > omin.y) {
        p->setYPos(pos.y+size.y);
      }
  }

  return Interface();
  
}

Interface Solid::interact(Player* p, Field*, SwitchHandler*) {
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

  if(xInt && pmin.x < omin.x) {
    p->setXPos(pos.x-p->getSize().y);
  }
  if(yInt && pmin.y < omin.y) {
    p->setYPos(pos.y-p->getSize().x);
  }
  if(xInt && pmin.x > omin.x) {
    p->setXPos(pos.x+size.x);
  }
  if(yInt && pmin.y > omin.y) {
    p->setYPos(pos.y+size.y);
  }

  return Interface();
}


CacheNodeAttributes Solid::draw(const TextureCache* cache) {
  // draw a solid object with no transforms
  // use obj_arg[0] to decide which texture to draw
  CacheNodeAttributes cna;
  switch(args[0]) {
  default:
    cna.srcImg = cache->reverseHash("stone");
  //other textures can be placed here for more design freedom
  }
  //no transforms, so tlist is left blank
  return cna;
}


Interface Solid::behave(SwitchHandler*) {
  //no special behaviors
  return Interface();
}
