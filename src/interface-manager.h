#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "icon.h"
#include "border-drawer.h"
#include "panel-creator.h"




//! A class that creates and manages the interface
class InterfaceManager {
public:
  //! Constructs the interface manager
  InterfaceManager();
  //! Returns the number of elements in the border
  int getBorderLen() const;
  ///! Returns the number of icons
  //size_t getNumIcons() const;
  //! Returns specified border element
  sf::Sprite& getBorderElem(int);
  //! Draws icons using passed RenderWindow.
  void drawIcons(sf::RenderWindow&);
  //! Initializes the interface using several parameters
  /*!
   *  Parameters: windowSize: size of the Gameplay window, in pixels
   *  Returns a sf::Vector2i which represents the size of the interface, in pixels.
   */
  sf::Vector2i initializeInterface(sf::Vector2i, int& msgOff, int& panOff, const Player*);
  //! Updates the interface, specifically the PanelCreator.
  void updateInterface(const Player*);
private:
  PanelCreator pc;
  BorderDrawer bd;
  sf::RectangleShape msgRect;
  sf::RectangleShape panelRect;
};

#endif
