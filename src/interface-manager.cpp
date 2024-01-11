#include "interface-manager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>

InterfaceManager::InterfaceManager() : bd(sf::Vector2i(16,16)) {
}

//this function needs to return:
/*
 * size of interface in pixels
 * message-box offset


 */
sf::Vector2i InterfaceManager::initializeInterface(sf::Vector2i windowSize, int& msgOff, int& panOff, const Player* pl, TextureCache* cache) {
  //initialize panel
  if(!pc.loadIcons("assets/interface/icons.txt")) {
    //error
    std::cout << "Error Loading panel icons\n";
  }
  pc.updateIcons(pl, cache);
  pc.createPanel();
  //determine panel size
  sf::Vector2i panelSize{pc.findPanelDimensions()};
  sf::Vector2i messageSize{288, 96};

  //alter sizes so the components fit together
  if(panelSize.y > 16+windowSize.y+messageSize.y) {
    //make message longer
    messageSize.y = panelSize.y - windowSize.y - 16;
  }
  else if(panelSize.y < 16+windowSize.y+messageSize.y) {
    //make panel longer
    panelSize.y = windowSize.y + 16 + messageSize.y;
  }

  messageSize.x = windowSize.x;


  //set offsets for Panel and message positions
  msgOff = 32+windowSize.y;
  panOff = 32+windowSize.x;

  //construct border
  bd.drawBorder(windowSize, panelSize, messageSize);

  // reposition each icon
  pc.fixPanelOffsets(panOff);
  
  panelRect.setSize(sf::Vector2f(panelSize.x, panelSize.y));
  panelRect.setPosition(windowSize.x + 32, 16);
  panelRect.setFillColor(sf::Color::Black);

  msgRect.setSize(sf::Vector2f(messageSize.x, messageSize.y));
  msgRect.setPosition(16, windowSize.y + 32);
  msgRect.setFillColor(sf::Color::Black);

  sf::Vector2i interfaceSize{48+windowSize.x+panelSize.x, 32+panelSize.y};
  return interfaceSize;

}

void InterfaceManager::updateInterface(const Player* p, TextureCache* cache) {
  pc.updateIcons(p, cache);
}
void InterfaceManager::drawIcons(sf::RenderWindow& w) {
  //draw rectangles
  w.draw(panelRect);
  w.draw(msgRect);
  pc.drawIcons(w);
}
int InterfaceManager::getBorderLen() const {
  return bd.border.size();
}
sf::Sprite& InterfaceManager::getBorderElem(int n) {
  return bd.border.at(n);
}
