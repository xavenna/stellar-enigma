#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

// This all needs to be sorted out into code/header, and separate heaeders
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "player.h"

class Icon : public sf::Sprite {
public:
  void setSize(sf::Vector2i);
  sf::Vector2i getSize() const;

  void (*update)();
  Icon(void(*up)(void));
private:
  int wid=0;
  int hei=0;
};


class PIcon : public Icon {
public:
  Player* player;
};


class BorderDrawer {
public:
  std::vector<sf::Sprite> border;

  void drawBorder(sf::Vector2i, sf::Vector2i, sf::Vector2i);

  BorderDrawer(sf::Vector2i tilesize);
private:
  sf::Texture corner;
  sf::Texture edge;
  sf::Texture joint;
  sf::Texture cross;
  sf::Vector2i tileSize;
  
};


class PanelCreator {
public:
  void createPanel();
  sf::Vector2i findPanelDimensions();

private:
  std::vector<Icon> icons;
};


class InterfaceManager {
public:
  InterfaceManager();
  int getBorderLen();
  sf::Sprite& getBorderElem(int);
  //! Initializes the interface using several parameters
  /*!
   *  Parameters: windowSize: size of the Gameplay window, in pixels
   *  Returns a sf::Vector2i which represents the size of the interface, in pixels.
   */
  sf::Vector2i initializeInterface(sf::Vector2i windowSize, int& msgOff, int& panOff);
private:
  PanelCreator pc;
  BorderDrawer bd;
};

#endif
