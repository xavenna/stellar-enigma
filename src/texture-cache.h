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
#include <algorithm>
#include "color.h"
#include "save.h"
#include "util.h"


//! A struct representing a transform for the texturecache to apply to the source image
struct Transform {
  //! The type of transform being applied to the source image
  enum Type {
    //! Sets the X-offset of the subrectangle
    /*!
     *  args: {x-offset}
     */
    Slide_X,
    //! Sets the Y-offset of the subrectangle
    /*!
     *  args: {Y-offset}
     */
    Slide_Y,
    //! Sets the width of the subrectangle
    /*!
     *  args: {width}
     */
    Set_Width,
    //! Sets the height of the subrectangle
    /*!
     *  args: {height}
     */
    Set_Height,
    //! Rotates the image before rendering
    /*!
     *  args: {How many 90 degree turns to rotate image counterclockwise}
     */
    Rotate,  //!< Rotates the image before rendering
    Tint_Color,  //!< Applies a tint (currently, converts pixels to HSV and alters hue)
    Tint_Mask, //!< Not implemented
    And_Mask, //!< Not implemented
    Displacement_Mask, //!< Not implemented
    Add_Circle, //!< Not implemented
    //! Superimposes text on image at specified position
    /*!
     *  args: {xpos, ypos, text size}
     *  By default, the text is displayed as requested. No wrapping is applied
     *  Save variables can be added to the text, using an escape sequence:
     *  %[typechar][varname]~, where typechar is 'b', 'i', 'f', 's'
     *  To print a %, use %%. 
     */
    Add_Text,
    //! Combination Subrectangle transform
    /*!
     *  args: {x-offset, y-offset, x-size, y-size}
     */
    SubRect
  };
  Type type; //!< The type of transform to apply
  std::array<int, 4> args;  //!< Arguments for the transform
  std::string text;  //!< Text variable for some transforms
  bool operator!=(Transform) const; //!< Transform inequality operator.
  bool operator==(Transform) const; //!< Transform equality operator.
};

//! A simple class that specifies a cache node. When an object requests a texture, it uses a CNA to do so.
/*!
 *  To request a texture, a CNA needs to be created. srcImg is initialized using TextureCache::reverseHash().
 *  This gets the proper id for a specified string. tList is a list of transforms to be applied.
 */
class CacheNodeAttributes {
public:
  unsigned srcImg; //!< Which image # is used
  std::vector<Transform> tList; //!< List of transforms to apply
  bool operator==(CacheNodeAttributes) const; //!< Checks if two CNAs fully match
};

//! A representation of a node within the texture cache.
class CacheNode : public CacheNodeAttributes {
public:
  sf::Texture tex; //!< The generated texture corresponding to the CNA.
  CacheNode(CacheNodeAttributes); //!< Constructs the Node from the CNA.
};


//! Texture manager, serves as an abstraction for textures. Allows transformations
/*!
 *  Upon initialization, loads specified images from the texturemap and stores them in the image vector
 *  When an object requests a texture, it calls getTexture() with a CNA representing the texture it wants
 *  The CNA specifies the image number and any transforms to be applied to it.
 *  Transforms include drawing additional geometry over the image, shifting the subrectangle used, rotating
 *  the image, etc. More transforms will be added eventually
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
  //! Determines the subcache number for a given CNA.
  int searchCache(CacheNodeAttributes) const; // This is private as it is only used internally
  //! A Const reference to the save controller. Used for text disp transforms
  const SaveController& save;
public:
  //! Returns the name of the nth image stored
  std::string hash(unsigned index) const;
  //! Returns the subcache id generated from the specified texture. Returns -1 if texture isn't registered
  int reverseHash(const std::string& value) const;
  //! Searches the cache for a texture matching the passed CNA. Generates and stores it if not found
  sf::Texture& getTexture(CacheNodeAttributes attr);
  //! Registers an image and a name to the texturecache. Probably should be private
  bool registerImage(sf::Image, const std::string&);
  //! Constructor: Initializes the cache with a texturemap file.
  /*!
   *  String argument passed is filename of texturemap.
   *  Additionally, passes in a reference to the Save Controller - used for some
   *  transforms
   */
  TextureCache(const std::string& name, SaveController&);
};
//! Get the subrectangle from a texturemap entry
sf::IntRect getRect(std::string);
//! Get the imagename from a texturemap entry
std::string getName(std::string);
//! Get the filename from a texturemap entry
std::string getFile(std::string);

//! Handles escape sequences for the Add_Text transform
void textEscape(std::string&, const SaveController&);
#endif
