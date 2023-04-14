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

#define ARG_NUM 1
//how many arguments a transform has

struct Transform {
  enum Type {
    Slide_X,
    Slide_Y,
    Set_Width,
    Set_Height,
    Rotate,
    Tint_Color,
    Tint_Mask,
    And_Mask,
    Displacement_Mask
  };
  Type type;
  std::array<int, ARG_NUM> args;
  bool operator!=(Transform) const;
  bool operator==(Transform) const;
};

class CacheNodeAttributes {
public:
  unsigned srcImg;
  std::vector<Transform> tList;
  bool operator==(CacheNodeAttributes) const;
};

class CacheNode : public CacheNodeAttributes {
public:
  sf::Texture tex;
  CacheNode(CacheNodeAttributes);
};


class TextureCache {
private:
  std::vector<std::string> imgNameHash;
  std::deque<std::deque<CacheNode>> cache;
  std::vector<sf::Image> images;
  std::vector<std::string> objectListing;
  std::vector<std::string> entityListing;
  std::vector<std::string> tileListing;
  int searchCache(CacheNodeAttributes) const; //!< This is private as it is only used internally
public:
  std::string hash(unsigned index) const;
  int objectFilenameHash(unsigned index) const;
  int tileFilenameHash(unsigned index) const;
  int entityFilenameHash(unsigned index) const;
  int reverseHash(const std::string& value) const;
  sf::Texture& getTexture(CacheNodeAttributes attr);
  bool registerImage(sf::Image, const std::string&);
  TextureCache(const std::string& name);
};
#endif
