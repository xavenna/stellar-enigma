#include "border-drawer.h"

//all argument sizes are supplied in pixels
//all dimensions are assumed to be multiples of the tilesize in the respective direction
void BorderDrawer::drawBorder(sf::Vector2i windowSize, sf::Vector2i panelSize, sf::Vector2i messageSize) {
  //I apoligize for how absolutely awful this function is.
  //Writing this made my brain hurt. I have a headache now.
  int winWid = windowSize.x / tileSize.x;
  int winHei = windowSize.y / tileSize.y;
  int panWid = panelSize.x / tileSize.x;
  int panHei = panelSize.y / tileSize.y;
  int msgHei = messageSize.y / tileSize.y;

  int bordLen = 6 + (3*winWid) + (2*panWid) + (3*panHei);
  border.resize(bordLen);

  border[0].setTexture(corner);
  border[0].setOrigin(static_cast<sf::Vector2f>(border[0].getTexture()->getSize()/2U));
  border[0].setRotation(0.f);
  border[0].setPosition(0+(tileSize.x*0.5f),0+(tileSize.y*0.5f));

  for(int i=1;i<winWid+1;i++) {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[0].getTexture()->getSize()/2U));
    border[i].setPosition(i*tileSize.x+(tileSize.x*0.5f),0+(tileSize.y*0.5f));
  }

  border[winWid+1].setTexture(joint);
  border[winWid+1].setOrigin(static_cast<sf::Vector2f>(border[0].getTexture()->getSize()/2U));
  border[winWid+1].setPosition((winWid+1)*tileSize.x+(tileSize.x*0.5f),0+(tileSize.y*0.5f));

  for(int i=winWid+2;i<winWid+2+panWid;i++) {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[0].getTexture()->getSize()/2U));
    border[i].setPosition(i*tileSize.x+(tileSize.x*0.5f),0+(tileSize.y*0.5f));
  }

  border[winWid+panWid+2].setTexture(corner);
  border[winWid+panWid+2].setOrigin(static_cast<sf::Vector2f>(border[winWid+panWid+2].getTexture()->getSize()/2U));
  border[winWid+panWid+2].setRotation(90);
  border[winWid+panWid+2].setPosition((winWid+panWid+2)*tileSize.x+(tileSize.x*0.5f),0+(tileSize.y*0.5f));

  for(int i=winWid+panWid+3;i<2*winWid+panWid+3;i++) {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[0].getTexture()->getSize()/2U));
    border[i].setPosition(tileSize.x*(i-winWid-panWid-2)+(tileSize.x*0.5f),tileSize.y*(winHei+1)+(tileSize.y*0.5f));
  }

  border[winWid*2+panWid+3].setTexture(corner);
  border[winWid*2+panWid+3].setOrigin(static_cast<sf::Vector2f>(border[winWid*2+panWid+3].getTexture()->getSize()/2U));
  border[winWid*2+panWid+3].setRotation(270);
  border[winWid*2+panWid+3].setPosition(0+(tileSize.x*0.5f),(panHei+1)*tileSize.y+(tileSize.y*0.5f));

  for(int i=winWid*2+panWid+4;i<3*winWid+panWid+4;i++) {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[0].getTexture()->getSize()/2U));
    border[i].setPosition((i-winWid*2-panWid-3)*tileSize.x+(tileSize.x*0.5f),(panHei+1)*tileSize.y+(tileSize.y*0.5f));
  }

  border[3*winWid+panWid+4].setTexture(joint);
  border[3*winWid+panWid+4].setOrigin(static_cast<sf::Vector2f>(border[3*winWid+panWid+4].getTexture()->getSize()/2U));
  border[3*winWid+panWid+4].setRotation(180);
  border[3*winWid+panWid+4].setPosition((winWid+1)*tileSize.x+(tileSize.x*0.5f),(panHei+1)*tileSize.y+(tileSize.y*0.5f));

  for(int i=3*winWid+panWid+5;i<3*winWid+2*panWid+5;i++) {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[0].getTexture()->getSize()/2U));
    border[i].setPosition((i-winWid*2-panWid-3)*tileSize.x+(tileSize.x*0.5f),(panHei+1)*tileSize.y+(tileSize.y*0.5f));
  }

  border[3*winWid+2*panWid+5].setTexture(corner);
  border[3*winWid+2*panWid+5].setOrigin(static_cast<sf::Vector2f>(border[3*winWid+2*panWid+5].getTexture()->getSize()/2U));
  border[3*winWid+2*panWid+5].setRotation(180);
  border[3*winWid+2*panWid+5].setPosition((winWid+panWid+2)*tileSize.x+(tileSize.x*0.5f), (panHei+1)*tileSize.y+(tileSize.y*0.5f));

  for(int i=3*winWid+2*panWid+6;i<3*winWid+2*panWid+winHei+6;i++) {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[i].getTexture()->getSize()/2U));
    border[i].setRotation(90);
    border[i].setPosition(0+(tileSize.x*0.5f),(i-3*winWid-2*panWid-5)*tileSize.y+(tileSize.y*0.5f));
  }

  border[3*winWid+2*panWid+winHei+6].setTexture(joint);
  border[3*winWid+2*panWid+winHei+6].setOrigin(static_cast<sf::Vector2f>(border[3*winWid+2*panWid+winHei+6].getTexture()->getSize()/2U));
  border[3*winWid+2*panWid+winHei+6].setRotation(270);
  border[3*winWid+2*panWid+winHei+6].setPosition(0+(tileSize.x*0.5f),(winHei+1)*tileSize.y+(tileSize.y*0.5f));

  for(int i=3*winWid+2*panWid+winHei+7;i<3*winWid+2*panWid+winHei+msgHei+7;i++) {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[i].getTexture()->getSize()/2U));
    border[i].setRotation(90);
    border[i].setPosition(0+(tileSize.x*0.5f),(i-3*winWid-2*panWid-5)*tileSize.y+(tileSize.y*0.5f));
  }
  
  for(int i=3*winWid+2*panWid+winHei+msgHei+7;i<3*winWid+2*panWid+2*winHei+msgHei+7;i++) {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[i].getTexture()->getSize()/2U));
    border[i].setRotation(90);
    border[i].setPosition((winWid+1)*tileSize.x+(tileSize.x*0.5f),(i-3*winWid-2*panWid-winHei-msgHei-6)*tileSize.y+(tileSize.y*0.5f));
  }

  border[3*winWid+2*panWid+2*winHei+msgHei+7].setTexture(joint);
  border[3*winWid+2*panWid+2*winHei+msgHei+7].setOrigin(static_cast<sf::Vector2f>(border[3*winWid+2*panWid+2*winHei+msgHei+7].getTexture()->getSize()/2U));
  border[3*winWid+2*panWid+2*winHei+msgHei+7].setRotation(90);
  border[3*winWid+2*panWid+2*winHei+msgHei+7].setPosition((winWid+1)*tileSize.x+(tileSize.x*0.5f),(winHei+1)*tileSize.y+(tileSize.y*0.5f));

  for(int i=3*winWid+2*panWid+2*winHei+msgHei+8;i<3*winWid+2*panWid+2*winHei+2*msgHei+8;i++)  {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[i].getTexture()->getSize()/2U));
    border[i].setRotation(90);
    border[i].setPosition((winWid+1)*tileSize.x+(tileSize.x*0.5f),(i-3*winWid-2*panWid-winHei-msgHei-6)*tileSize.y+(tileSize.y*0.5f));
  }

  for(int i=3*winWid+2*panWid+2*winHei+2*msgHei+8;i<3*winWid+2*panWid+2*winHei+2*msgHei+panHei+8;i++)  {
    border[i].setTexture(edge);
    border[i].setOrigin(static_cast<sf::Vector2f>(border[i].getTexture()->getSize()/2U));
    border[i].setRotation(90);
    border[i].setPosition((winWid+panWid+2)*tileSize.x+(tileSize.x*0.5f), (i-3*winWid-2*panWid-2*winHei-2*msgHei-7)*tileSize.y+(tileSize.y*0.5f));
  }
}

BorderDrawer::BorderDrawer(sf::Vector2i ts) {
  corner.loadFromFile("assets/interface/corner.png");
  edge.loadFromFile("assets/interface/edge.png");
  joint.loadFromFile("assets/interface/joint.png");
  cross.loadFromFile("assets/interface/cross.png");
  tileSize = ts;
}