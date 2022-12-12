#include "interface-manager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>

InterfaceManager::InterfaceManager() : bd(sf::Vector2i(36,36)) {
}

//this function needs to return:
/*
 * size of interface in pixels
 * message-box offset


 */
sf::Vector2i InterfaceManager::initializeInterface(sf::Vector2i windowSize, int& msgOff, int& panOff, const Player* pl) {
  //initialize panel
  if(!pc.loadIcons("assets/interface/icons.txt")) {
    //error
    std::cout << "Error Loading panel icons\n";
  }
  pc.updateIcons(pl);
  pc.createPanel();
  //determine panel size
  sf::Vector2i panelSize{pc.findPanelDimensions()};
  sf::Vector2i messageSize{288, 108};

  //alter sizes so the components fit together
  if(panelSize.y > 36+windowSize.y+messageSize.y) {
    //make message longer
    messageSize.y = panelSize.y - windowSize.y - 36;
  }
  else if(panelSize.y < 36+windowSize.y+messageSize.y) {
    //make panel longer
    panelSize.y = windowSize.y + 36 + messageSize.y;
  }

  messageSize.x = windowSize.x;


  //set offsets for Panel and message positions
  msgOff = 72+windowSize.y;
  panOff = 72+windowSize.x;

  //construct border
  bd.drawBorder(windowSize, panelSize, messageSize);

  // reposition each icon
  pc.fixPanelOffsets(panOff);
  
  sf::Vector2i interfaceSize{108+windowSize.x+panelSize.x, 72+panelSize.y};
  return interfaceSize;

}

void InterfaceManager::updateInterface(const Player* p) {
  pc.updateIcons(p);
}
void InterfaceManager::drawIcons(sf::RenderWindow& w) {
  pc.drawIcons(w);
}
int InterfaceManager::getBorderLen() const {
  return bd.border.size();
}
sf::Sprite& InterfaceManager::getBorderElem(int n) {
  return bd.border.at(n);
}
