#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>

//! A class that is used to display menus
/*!
 *  Currently, its functionality is fairly limited
 */
class Menu {
public:
  //! The sprite that is drawn when the menu is activated
  sf::Sprite splash;
  //! The texture that splash is set to
  sf::Texture spT;

  //! Determines what happens when enter is pressed.
  /*!
   *  -1 specifies a program close, 1 specifies switching to mode 1
   */
  int onPress=1;

};

#endif
