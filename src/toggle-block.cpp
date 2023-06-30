#include "toggle-block.h"


ToggleBlock::ToggleBlock() : Object() {
  /*  once arg loading is implemented, uncomment this
  vars[0] = args[0];
   */
  vars[1] = 0;
  args[1] = 15;  //this is temporary, until arg loading works 
}
ToggleBlock::ToggleBlock(Object ob) : Object(ob) {
  /*  once arg loading is implemented, uncomment this
  vars[0] = args[0];
   */
  vars[1] = 0;
  args[1] = 15;  //this is temporary, until arg loading works 
}
ToggleBlock::ToggleBlock(int x, int y, int wid, int hei, int i, int v, bool sol, const std::string& txt, std::array<int, 8> a) : Object(x, y, wid, hei, i, v, sol, txt, a) {
  /*  once arg loading is implemented, uncomment this
  vars[0] = args[0];
   */
  vars[1] = 0;
  args[1] = 15;  //this is temporary, until arg loading works 
}
Interface ToggleBlock::interact(Player* p, Field*, bool dryRun) {
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

  if(!dryRun) {
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

  if(xInt || yInt) {
    //toggle state
    if(!vars[1]) {
      vars[0] = !vars[0];
      vars[1] = args[1];
    }
  }

  if(xInt && pmin.x < omin.x) {
    return Interface(sf::Vector2i(pos.x-p->getSize().y, p->getPos().y), "", "");
  }
  if(yInt && pmin.y < omin.y) {
    return Interface(sf::Vector2i(p->getPos().y, pos.y-p->getSize().x), "", "");
  }
  if(xInt && pmin.x > omin.x) {
    return Interface(sf::Vector2i(pos.x+size.x, p->getPos().y), "", "");
  }
  if(yInt && pmin.y > omin.y) {
    return Interface(sf::Vector2i(p->getPos().x, pos.y+size.y), "", "");
  }

  return Interface(p->getPos(), "", "");
}

CacheNodeAttributes ToggleBlock::draw(const TextureCache* cache) {
  // check value of some internal state, and draw using obj_args
  CacheNodeAttributes cna;
  if(vars[0] == 1) {
    cna.srcImg = cache->reverseHash("on-barr-big");
  }
  else {
    cna.srcImg = cache->reverseHash("off-barr-big");
  }

  //no transforms, so tlist is left blank
  return cna;
}

Interface ToggleBlock::behave() {
  if(vars[1] > 0) {
    vars[1]--;
  }
  return Interface(pos, "", "");
}
