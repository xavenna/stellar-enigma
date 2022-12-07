#ifndef PANEL_CREATOR_H
#define PANEL_CREATOR_H
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "player.h"
#include "icon.h"
#include "texturemap.h"

class PanelCreator {
public:
  void fixPanelOffsets(int);
  void createPanel();
  bool loadIcons(std::string);
  sf::Vector2u findPanelDimensions();
  bool isValidCallback(std::string);
  void updateIcons(const Player*);
  void drawIcons(sf::RenderWindow&);
  TextureMap& tm;
  PanelCreator(TextureMap&);
private:
  std::vector<Icon> icons;
  std::vector<PIcon> picons; //there may be a better way to do this
  sf::Vector2u size;
};

namespace fs=std::filesystem;

#endif
