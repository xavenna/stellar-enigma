#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H
// separate this out into many different files once this works
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <deque>
#include <string>
#include <iostream>
#include <fstream>
#include "color.h"

#define ARG_NUM 4
//how many arguments a transform has

struct Transform {
  //! The type of transform being applied to the source image
  enum Type {
    Slide_X,  //!< Sets the subrectangle X offset
    Slide_Y,  //!< Sets the subrectangle Y offset
    Set_Width,  //!< Sets the width of the subrectangle
    Set_Height,  //!< Sets the height of the subrectangle
    Rotate,  //!< Rotates the image before rendering
    Tint_Color,  //!< Applies a tint (currently, converts pixels to HSV and alters hue)
    Tint_Mask,
    And_Mask,
    Displacement_Mask,
    Add_Circle,
    Add_Text  //!< Superimposes text on image at specified position
  };
  Type type;
  std::array<int, ARG_NUM> args;
  std::string text;  //!< Text variable for some transforms
  bool operator!=(Transform) const;
  bool operator==(Transform) const;
};

//! A simple class that specifies a cache node. When an object requests a texture, it uses a CNA to do so.
/*!
 *  To request a texture, a CNA needs to be created. srcImg is initialized using TextureCache::reverseHash().
 *  This gets the proper id for a specified string. tList is a list of transforms to be applied.
 */
class CacheNodeAttributes {
public:
  unsigned srcImg;
  std::vector<Transform> tList;
  bool operator==(CacheNodeAttributes) const;
};

//! A representation of a node within the texture cache.
class CacheNode : public CacheNodeAttributes {
public:
  sf::Texture tex;
  CacheNode(CacheNodeAttributes);
};


//! Texture manager, serves as an abstraction for textures. Allows transformations
/*!
 *  Upon initialization, loads specified images from the texturemap and stores them in the image vector
 *  When an object requests a texture, it calls getTexture() with a CNA representing the texture it wants
 *  The CNA specifies the image number and any transforms to be applied to it.
 *  Transforms include drawing additional geometry over the image, shifting the subrectangle used, rotating
 *  the image, etc.
 *
 *  To get the image number, use reverseHash(textureName), where textureName is the corresponding texture
 *  defined in the texturemap.
 *
 *  Texturemap format:  (This will be replaced with a cleaner format eventually)
 *  Each line in the texturemap represents a texture. The format is as follows:
 *  tilename`filename`xpos,ypos`wid,hei
 *  tilename is the internal name to register in imgNameHash
 *  filename is the source file to read the image from
 *  xpos,ypos is an ordered pair defining the top-left corner of the subrectangle to use. Defaults to 0,0
 *  wid,hei is an ordered pair defining the size of the subrectangle to use. Defaults to image size
 */
class TextureCache {
private:
  //! Names associated with registered images
  std::vector<std::string> imgNameHash;
  //! The cache data structure. Each sub-deque is a sub-cache, which contains derivatives of one image
  std::deque<std::deque<CacheNode>> cache;
  //! All images that have been registered
  std::vector<sf::Image> images;
  //! These are deprecated, an artifact from an old system
  std::vector<std::string> objectListing;
  std::vector<std::string> entityListing;
  std::vector<std::string> tileListing;
  //! ...
  int searchCache(CacheNodeAttributes) const; // This is private as it is only used internally
public:
  //! Returns the name of 
  std::string hash(unsigned index) const;
  int objectFilenameHash(unsigned index) const;
  int tileFilenameHash(unsigned index) const;
  int entityFilenameHash(unsigned index) const;
  //! Returns the subcache id generated from the specified texture. Returns -1 if texture isn't registered
  int reverseHash(const std::string& value) const;
  //! Searches the cache for a texture matching the passed CNA. Generates and stores it if not found
  sf::Texture& getTexture(CacheNodeAttributes attr);
  //! Registers an image and a name to the texturecache. Probably should be private
  bool registerImage(sf::Image, const std::string&);
  //! Constructor: Initializes the cache with a texturemap file. Argument passed is filename of texturemap.
  TextureCache(const std::string& name);
};

sf::IntRect getRect(std::string);
std::string getName(std::string);
std::string getFile(std::string);
#endif
