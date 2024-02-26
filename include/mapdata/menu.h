#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include "utility/texture-cache.h"
#include "level/level.h"


//! Defines the attributes of a specific menu
struct MenuTemplate {
  //! Specifies exit codes for each interaction key
  /*!
   *  An interaction key, in this context, is a key that can incite an effect in a menu
   *  These keys are, in order,
   *  Enter, 1, 2, 3, 4, Esc, Space
   *  See Menu class documentation for more about exit codes
   */
  std::array<int, 7> exitBehavior;
  //! Supplemental string arg for certain exit codes.
  std::array<std::string, 7> exitStrArgs;
  //! The menu template's name
  /*! 
   *  This is shadowed as the string argument in the map within Menu.
   *  Having it be here first simplifies the initialization process
   */
  std::string name;
  //! Specifies which image this menu uses as a base
  std::string baseImage;
  //! Transforms to apply to specified image
  std::vector<Transform> tlist;
  //! Constructs the MenuTemplate from a string representation of one.
  MenuTemplate(const std::string&);
};


//! A class that is used to display menus
/*!
 *  Exit code Documentation:
 *  -1: Close program
 *  0: Do nothing
 *  1: Enter mode 1 [With player placed at last (or preset) location]
 *  2: Play a cutscene, specified by text arg, then switch to mode 1
 *  3: Switch to a different menu, specified by text arg
 *  4: Enter mode 1, reload map from file [text arg specifies which one]
 *  
 *  There will be more eventually
 *
 */
class Menu : public sf::Sprite {
public:

  //! A list of all loaded menu templates
  std::map<std::string, MenuTemplate> menus;

  //! Which menutemplate is currently in use
  std::string currentTemplate;

  //! Load menus from list
  bool initialize();
  //! Uses the specified template to fetch a texture from the cache
  void assignTexture(TextureCache&);
  //! Sets menu to use the specified template, if stored. Returns false if not found
  bool loadTemplate(const std::string&);

  //! Gets press code for specified index
  int getCode(unsigned) const;
  //! Gets specified text argument
  std::string getTextArg(unsigned) const;
};

//! Parses a string, fixes escape sequences: "\\n" becomes "\n"
void fixEscape(std::string&);

#endif
