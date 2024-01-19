#include "spike.h"

Spike::Spike(int uid) : Solid(uid) {}

Interface Spike::interact(Player* p, Field*, SwitchHandler*) {
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

  bool hurt = false;
  if(xInt && pmin.x < omin.x) {
    p->setXPos(pos.x-p->getSize().y);
    hurt = true;
  }
  if(yInt && pmin.y < omin.y) {
    p->setYPos(pos.y-p->getSize().x);
    hurt = true;
  }
  if(xInt && pmin.x > omin.x) {
    p->setXPos(pos.x+size.x);
    hurt = true;
  }
  if(yInt && pmin.y > omin.y) {
    p->setYPos(pos.y+size.y);
    hurt = true;
  }
  if(hurt) {
    if(p->getCooldown() == 0) {
      p->modifyHealth(-1);
      p->damaged = true;
    }
  }
  return Interface();
}

CacheNodeAttributes Spike::draw(const TextureCache* cache) {
  // draw a cactus with no transforms
  CacheNodeAttributes cna;
  cna.srcImg = cache->reverseHash("cactus");
  //no transforms, so tlist is left blank
  return cna;
}
