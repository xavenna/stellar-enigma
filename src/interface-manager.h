#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

// This all needs to be sorted out into code/header, and separate heaeders
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "icon.h"
#include "border-drawer.h"
#include "panel-creator.h"
#include "texturemap.h"





class InterfaceManager {
public:
  InterfaceManager();
  int getBorderLen();
  size_t getNumIcons();
  sf::Sprite& getBorderElem(int);
  void drawIcons(sf::RenderWindow&);
  //! Initializes the interface using several parameters
  /*!
   *  Parameters: windowSize: size of the Gameplay window, in pixels
   *  Returns a sf::Vector2i which represents the size of the interface, in pixels.
   */
  sf::Vector2i initializeInterface(sf::Vector2i, int& msgOff, int& panOff, const Player*);
  void updateInterface(const Player*);
private:
  PanelCreator pc;
  BorderDrawer bd;
};

#endif
