#ifndef BORDER_DRAWER_H
#define BORDER_DRAWER_H
#include <vector>
#include <SFML/Graphics.hpp>

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

#endif
