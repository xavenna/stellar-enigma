#ifndef ICON_H
#define ICON_H

#include <SFML/Graphics.hpp>
#include "player.h"

class Icon : public sf::Sprite {
public:
  void setSize(sf::Vector2u);
  void setPos(sf::Vector2i);
  void setId(unsigned);
  sf::Vector2u getSize() const;
  sf::Vector2i getPos() const;
  unsigned getId() const;
  virtual void update();
  Icon(std::string);
  sf::Texture tex;
protected:
  sf::Vector2u size;
  sf::Vector2i pos;
  std::string callback;
  unsigned id;
};


class PIcon : public Icon {
public:
  Player* player;
  void update(const Player*);
  PIcon(std::string);
};


void nullAct();
void showHealthAsNumber(const Player*, sf::Texture&);
#endif
