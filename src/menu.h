#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include "texture-cache.h"

//! A class that is used to display menus
/*!
 *  Currently, its functionality is fairly limited
 */
class Menu {
public:
  //! The sprite that is drawn when the menu is activated
  sf::Sprite splash;
  //! The texture that splash is set to. Deprecated
  sf::Texture spT;

  std::string baseImage;
  std::vector<Transform> tList;

  //! Determines what happens when enter is pressed.
  /*!
   *  -1 specifies a program close, 1 specifies switching to mode 1
   */
  int onPress=1;
  void initialize();
  void assignTexture(TextureCache&);
};

#endif
