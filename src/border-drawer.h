#ifndef BORDER_DRAWER_H
#define BORDER_DRAWER_H
#include <vector>
#include <SFML/Graphics.hpp>

//! Handles the drawing of the border
/*!
 *  This class creates the border from tiles. It dynamically creates the border based on the
 *  sizes of the message, panel, and game window.
 */
class BorderDrawer {
public:
  //! A container for the actual sprites that make up the border
  std::vector<sf::Sprite> border;

  //! Creates the border, using the specified sizes for components
  void drawBorder(sf::Vector2i windowSize, sf::Vector2i panelSize, sf::Vector2i messageSize);

  //! Constructs the border with the size of the individual tiles
  BorderDrawer(sf::Vector2i tilesize);
private:
  sf::Texture corner;
  sf::Texture edge;
  sf::Texture joint;
  sf::Texture cross;
  sf::Vector2i tileSize;
  
};

#endif
