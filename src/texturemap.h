#ifndef TEXTUREMAP_H
#define TEXTUREMAP_H

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <fstream>

//!  A class responsible for assigning textures
/*!  A texturemap is responsible for assigning textures to sf::Sprites.
 *   It contains a vector of sf::Textures, which can be assigned using
 *   encapsulated methods. It loads the textures from images, using a
 *   texturemap file (*.tm). This is a plain-text file that is composed
 *   of a list of filenames, which are loaded in order. When setting a 
 *   texture for a Node, the idTh texture is chosen.
 */
class TextureMap {
  int objectOffset=0; //!< The point where object textures begin
  int entityOffset=0; //!< The point where entity textures begin
  std::vector<sf::Texture> mapping; //!< the vector that stores Textures
public:
  //! returns a reference to the texture with number specified by argument
  sf::Texture& getTexture(const int&);

  //! sets a texture in mapping to supplied texture
  /*!
   *  I don't believe this is used anywhere, as textures need to be references.
   *  So don't use this for now
   */
  void assign(int, sf::Texture);
  //! Sets up the texturemap using the supplied filename
  /*!
   *  The file is assumed to be located in ./assets/texturemap/
   */
  bool initialize(const std::string&);
  //! A function which returns the number of textures stored
  int size() const;

  //! Gets the object offset
  int getObjOff() const;
  //! Gets the entity offset
  int getEntOff() const;

  //! Constructor that initializes the textures with supplied argument
  TextureMap(const std::string&);
  //! The default constructor, takes no arguments
  TextureMap();
};

#endif
