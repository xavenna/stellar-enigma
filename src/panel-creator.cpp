#include "panel-creator.h"

sf::Vector2u PanelCreator::findPanelDimensions() {
  //return sf::Vector2i(72, 432);
  return size;
}

void PanelCreator::fixPanelOffsets(int panOff) {
  for(auto& x : icons) {
    x.setPosition(x.getPosition().x+panOff, x.getPosition().y+16);
  }
  for(auto& x : picons) {
    x.setPosition(x.getPosition().x+panOff, x.getPosition().y+16);
  }
}

void PanelCreator::createPanel() {
  //i'm not sure how to do this
  //let's do a simplified version for now:
  //this is in no way optimal
  sf::Vector2u currentPos{0,0};
  unsigned maxWid=0;
  for(auto& x : icons) {
    //place this icon on the panel, setPos
    x.setPosition(currentPos.x, currentPos.y);
    x.setPos(sf::Vector2i(currentPos.x, currentPos.y));
    currentPos.y += x.getSize().y;
    maxWid = x.getSize().x > maxWid ? x.getSize().x : maxWid;
  }
  for(auto& x : picons) {
    //place this icon on the panel, setPos
    x.setPosition(currentPos.x, currentPos.y);
    x.setPos(sf::Vector2i(currentPos.x, currentPos.y));
    currentPos.y += x.getSize().y;
    maxWid = x.getSize().x > maxWid ? x.getSize().x : maxWid;
  }
  size = currentPos;
  size.x = maxWid;
}


void PanelCreator::updateIcons(const Player* p, TextureCache* cache) {
  for(auto& x : icons) {
    x.update();
    x.setSize(x.getTexture()->getSize());
    x.setTexture(x.tex);
  }
  for(auto& x : picons) {
    x.update(p, cache);
    x.setSize(x.getTexture()->getSize());
    x.setTexture(x.tex, cache);
  }
}

bool PanelCreator::isValidCallback(const std::string& call) {
  std::vector<std::string> calls = {"null", "playerNull", "playerHealth", "playerScore"};
  return std::find(calls.begin(), calls.end(), call) != calls.end();
  //yes, this is almost surely not the best way to do this, but it is simple
}
void PanelCreator::drawIcons(sf::RenderWindow& w) {
  for(auto& x : icons) {
    w.draw(x);
  }
  for(auto& x : picons) {
    w.draw(x);
  }
}
/* Contents of an icon line:
 *  Type {n-normal, p-playerlink}
 *  Name
 *  Reference to behavior function
 *  initial texture (size is determined from this)
 
 *  syntax: type`name`callback`texture
 */
bool PanelCreator::loadIcons(const std::string& file) {
  //load icons from: /assets/interface/icons.txt
  std::ifstream load(file);
  std::string line;
  std::string name, callback, texture, type;
  if(!load.is_open()) {
    return false;
  }
  while(load.peek() != EOF) {
    std::getline(load, line);

    type = line.substr(0, line.find('`'));
    line = line.substr(1+line.find('`'));
    name = line.substr(0, line.find('`'));
    line = line.substr(1+line.find('`'));
    callback = line.substr(0, line.find('`'));
    line = line.substr(1+line.find('`'));
    texture = line;

    //verify fields
    //check if specified callback exists
    if(!isValidCallback(callback)) {
      std::cout << "Error: invalid callback name '" << callback << "'\n";
      return false;
    }

    //check if specified image file exists
    fs::path tex = "assets/interface";
    tex /= texture;
    if(!fs::exists(tex)) {
      std::cout << "Error: invalid initial texture name\n";
      return false;
    }

    //check if type is valid
    if(type == "n") {
      //normal icon
      icons.push_back(Icon(callback));
      icons.back().tex.loadFromFile(tex.string());
      icons.back().setTexture(icons.back().tex);
      icons.back().setSize(icons.back().tex.getSize());
    }
    else if(type == "p") {
      //player-linked icon
      picons.push_back(PIcon(callback));
      picons.back().tex.loadFromFile(tex.string());
      picons.back().setTexture(picons.back().tex);
      picons.back().setSize(picons.back().tex.getSize());
    }
    else {
      std::cout << "Error: invalid icon type\n";
      return false;
    }


  }
  return true;
}

PanelCreator::PanelCreator() {
}
