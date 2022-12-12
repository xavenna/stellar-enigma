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

//! A class that handles the Panel component of the interface
/*!
 *  This class is responsible for constructing and updating the panel of stellar-enigma.
 *  The panel is a rectangular area of the interface that contains a number of icons. Each
 *  icon is updated during gameplay to reflect changes in the gameplay. 
 */
class PanelCreator {
public:
  //! Adjusts the position of each icon so that they display in the correct location.
  void fixPanelOffsets(int);
  //! Assigns positions to each icon
  void createPanel();
  //! Loads all icons from the specified file
  bool loadIcons(const std::string&);
  //! Returns the dimensions of the panel.
  sf::Vector2u findPanelDimensions();
  //! Determines if the supplied string represents a valid callback function
  bool isValidCallback(const std::string&);
  //! Updates all icons, using the passed pointers to determine elements of the game's state
  /*!
   *  For each icon contained within, the function uses the icon's 'callback' element to
   *  determine which function to use to update the icon.
   */
  void updateIcons(const Player*);
  //! Draws all icons to the passed RenderWindow
  void drawIcons(sf::RenderWindow&);
  //! Empty constructor
  PanelCreator();
private:
  std::vector<Icon> icons;
  std::vector<PIcon> picons; //there may be a better way to do this
  sf::Vector2u size;
};

namespace fs=std::filesystem;

#endif
