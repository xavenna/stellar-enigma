#ifndef MODE_H
#define MODE_H

#include <deque>
#include <SFML/Graphics.hpp>
#include "stellar.h"

class ModeSwitcher {  //this may need a cleanup function for switching modes
private:
  int mode;
  std::deque<sf::Keyboard::Key> keyList;  //this might not be used;
public:
  int getMode();
  void setMode(int);
  void addKey(sf::Keyboard::Key);
  sf::Keyboard::Key getLastKey();
  bool getLastKey(sf::Keyboard::Key&);
  ModeSwitcher();
};


#endif
